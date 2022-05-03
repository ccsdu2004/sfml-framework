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
    bool running = false;
    //std::set<StatePointer> stateSet;

    struct StateTransition {
        std::shared_ptr<State> to;
        std::function<bool()> shouldDoIt;
        int probability = 100;
    };

    std::map<StatePointer, std::list<StateTransition>> stateTransitions;
};

StateMachine::StateMachine():
    data(new StateMachineData)
{
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
    if(state) {
        data->initState = state;
        data->initState->setStateMachine(shared_from_this());
    }
}

void StateMachine::setErrorState(StatePointer state)
{
    if(state) {
        data->errorState = state;
        data->errorState->setStateMachine(shared_from_this());
    }
}

void StateMachine::addTransition(StatePointer from, StatePointer to, std::function<bool()> fn, int probability)
{
    if(from && to && fn && probability > 0) {
        from->setStateMachine(shared_from_this());
        to->setStateMachine(shared_from_this());

        probability = clamp(probability, 1, 100);
        StateMachineData::StateTransition transition;
        transition.shouldDoIt = fn;
        transition.probability = probability;
        transition.to = to;

        auto find = data->stateTransitions.find(from);
        if(find != data->stateTransitions.end())
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
    if(data->currentState)
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
    if(data->currentState) {
        data->currentState->update(deltaTime);
        if(data->running && data->currentState->isFinished()) {
            data->currentState->leave();
            data->updateCurrentState();
        }
    }
}

bool StateMachine::start()
{
    if(!data->initState || !data->errorState)
        return false;

    data->running = true;
    data->currentState = data->initState;
    data->currentState->enter();
    return true;
}

void StateMachineData::updateCurrentState()
{
    if(currentState) {
        auto find = stateTransitions.find(currentState);
        if(find != stateTransitions.end()) {
            auto list = find->second;

            auto itr = list.begin();
            while(itr != list.end()) {
                if(shouldRandDoIt(itr->probability)) {
                    if(itr->shouldDoIt()) {
                        currentState = itr->to;
                        currentState->enter();
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
    currentState->leave();
    currentState = errorState;
    currentState->enter();
}



