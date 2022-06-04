#pragma once
#include <functional>
#include <Def.h>
#include <NameHolder.h>
#include <Message.h>

enum StateMessage_Event {
    StateMessage_Event_Switch = 0,
    StateMessage_Event_Max
};

class StateMessage : public Message
{
public:
    StateMessage():
        Message(Message_STATE)
    {
    }

    virtual ~StateMessage()
    {
    }
};

class StateSwitchMessage : public StateMessage
{
public:
    explicit StateSwitchMessage(const std::string& inputState):
        targetState(inputState)
    {
    }

    StateSwitchMessage(const std::string& inputState, std::function<bool()> inputChecker):
        targetState(inputState),
        checker(inputChecker)
    {
    }

    std::string getTargetState()const
    {
        return targetState;
    }

    bool shouldSwitch()
    {
        return checker ? checker() : true;
    }
private:
    std::string targetState;
    std::function<bool()> checker;
};

class State : public NameHolder, public MessageReceiver
{
    friend class StateMachine;
public:
    State();
    State(ObjectPointer inputObject);
    virtual ~State();
public:
    void setTarget(ObjectPointer inputObject);
    std::weak_ptr<Object> getTarget()const;

    std::weak_ptr<StateMachine> getStateMachine()const;

    void enter();
    void leave();

    virtual void update(float deltaTime);

    void setFinished();
    bool isFinished()const;
private:
    virtual void onEnter();
    virtual void onLeave();
    void setStateMachine(std::shared_ptr<StateMachine> statemachine);
private:
    std::unique_ptr<class StateData> data;
};

using StatePointer = std::shared_ptr<State>;

class StateMachine : public MessageReceiver, public std::enable_shared_from_this<StateMachine>
{
    friend class StateMachineListener;
public:
    StateMachine();
    virtual ~StateMachine();
public:
    StatePointer getCurrentState()const;
    void setInitState(StatePointer state);
    void addState(StatePointer state);

    void setErrorState(StatePointer state);
    void addTransition(StatePointer from, StatePointer to, std::function<bool()> fn,
                       int probability = 100);
public:
    bool start();
    bool isRunning()const;
    void stop();
    void clear();

    void update(float deltaTime);
protected:
    std::unique_ptr<class StateMachineData> data;
};
