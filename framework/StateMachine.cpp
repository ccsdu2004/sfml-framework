#include <list>
#include <set>
#include <map>
#include <Util.h>
#include <Message.h>
#include <StateMachine.h>

class StateData
{
public:
    std::weak_ptr<Object> target;
    std::weak_ptr<StateMachine> stateMachine;
    bool finished = false;
};

State::State():
    data(new StateData())
{
}

State::State(ObjectPointer inputObject):
    data(new StateData())
{
    data->target = inputObject;
}

State::~State()
{
}

void State::setTarget(ObjectPointer inputObject)
{
    data->target = inputObject;
}

std::weak_ptr<Object> State::getTarget() const
{
    return data->target;
}

std::weak_ptr<StateMachine> State::getStateMachine() const
{
    return data->stateMachine;
}

void State::enter()
{
    data->finished = false;
    onEnter();
}

void State::leave()
{
    onLeave();
}

void State::update(float deltaTime)
{
    (void)deltaTime;
}

void State::setFinished()
{
    data->finished = true;
}

bool State::isFinished() const
{
    return data->finished;
}

void State::onEnter()
{
}

void State::onLeave()
{
}

void State::setStateMachine(std::shared_ptr<StateMachine> statemachine)
{
    data->stateMachine = statemachine;
}

class StateMachineData
{
public:
    void updateCurrentState();
    void enterErrorState();

    StatePointer initState;
    StatePointer errorState;
    StatePointer currentState;
    std::map<std::string, StatePointer> states;
    bool running = false;

    bool stateRunning = false;

    struct StateTransition {
        std::shared_ptr<State> to;
        std::function<bool()> shouldDoIt;
        int probability = 100;
    };

    std::map<StatePointer, std::list<StateTransition>> stateTransitions;
};

class StateMachineListener : public MessageListener
{
public:
    StateMachineListener() = delete;
    StateMachineListener(StateMachine &machine):
        stateMachine(machine)
    {
    }
public:
    bool onListener(std::shared_ptr<Message> message)
    {
        if (message->getType() != Message_STATE)
            return false;

        auto stateMessage = std::dynamic_pointer_cast<StateSwitchMessage>(message);
        bool flag = stateMessage->shouldSwitch();
        if (!flag)
            return false;

        auto target = stateMessage->getTargetState();

        auto currentState = stateMachine.getCurrentState();
        if (currentState && currentState->getName() == target)
            return false;

        auto find = stateMachine.data->states.find(target);
        if (find == stateMachine.data->states.end())
            return false;

        currentState->leave();
        find->second->enter();
        stateMachine.data->currentState = find->second;

        return true;
    }
private:
    StateMachine &stateMachine;
};

StateMachine::StateMachine():
    data(new StateMachineData)
{
    auto listener = std::make_shared<StateMachineListener>(*this);
    addMessageListener(listener);
}

StateMachine::~StateMachine()
{
}

StatePointer StateMachine::getCurrentState()const
{
    return data->currentState;
}

void StateMachine::setInitState(StatePointer state)
{
    if (state) {
        data->initState = state;
        data->initState->setStateMachine(shared_from_this());
    }
}

void StateMachine::addState(StatePointer state)
{
    if (!state)
        return;

    if (data->states.find(state->getName()) != data->states.end())
        return;

    data->states.insert(std::make_pair(state->getName(), state));
}


void StateMachine::setErrorState(StatePointer state)
{
    if (state) {
        data->errorState = state;
        data->errorState->setStateMachine(shared_from_this());
    }
}

void StateMachine::addTransition(StatePointer from, StatePointer to, std::function<bool()> fn,
                                 int probability)
{
    if (from && to && fn && probability > 0) {
        from->setStateMachine(shared_from_this());
        to->setStateMachine(shared_from_this());

        probability = clamp(probability, 1, 100);
        StateMachineData::StateTransition transition;
        transition.shouldDoIt = fn;
        transition.probability = probability;
        transition.to = to;

        auto find = data->stateTransitions.find(from);
        if (find != data->stateTransitions.end())
            find->second.push_back(transition);
        else {
            std::list<StateMachineData::StateTransition> list = {transition};
            data->stateTransitions.insert(std::make_pair(from, list));
        }
    }
}

bool StateMachine::isRunning()const
{
    return data->running;
}

void StateMachine::stop()
{
    if (data->currentState)
        data->currentState->leave();
    data->running = false;
}

void StateMachine::clear()
{
    data->currentState = nullptr;
    data->errorState = nullptr;
    data->initState = nullptr;
    data->stateTransitions.clear();
    data->running = false;
}

void StateMachine::update(float deltaTime)
{
    if (data->currentState) {
        data->currentState->update(deltaTime);
        if (data->running && data->currentState->isFinished()) {
            if (data->stateRunning) {
                data->currentState->leave();
                data->stateRunning = false;
            }
            data->updateCurrentState();
        }
    }
}

bool StateMachine::start()
{
    if (!data->initState || !data->errorState)
        return false;

    data->running = true;
    data->currentState = data->initState;
    data->currentState->enter();
    data->stateRunning = true;
    return true;
}

void StateMachineData::updateCurrentState()
{
    if (currentState) {
        auto find = stateTransitions.find(currentState);
        if (find != stateTransitions.end()) {
            auto list = find->second;

            auto itr = list.begin();
            while (itr != list.end()) {
                if (shouldRandDoIt(itr->probability)) {
                    if (itr->shouldDoIt()) {
                        currentState = itr->to;
                        currentState->enter();
                        stateRunning = true;
                        break;
                    }
                }
                itr ++;
            }

        } else
            enterErrorState();
    }
}

void StateMachineData::enterErrorState()
{
    stateRunning = false;
    currentState->leave();
    currentState = errorState;
    currentState->enter();
    stateRunning = true;
}

