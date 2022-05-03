#pragma once
#include <functional>
#include <Def.h>
#include <NameHolder.h>

class State : public NameHolder
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

class StateMachine : public std::enable_shared_from_this<StateMachine>
{
public:
    StateMachine();
    virtual ~StateMachine();
public:
    StatePointer getCurrentState()const;
    void setInitState(StatePointer state);
    void setErrorState(StatePointer state);
    void addTransition(StatePointer from, StatePointer to, std::function<bool()> fn, int probability = 100);
public:
    bool start();
    bool isRunning()const;
    void stop();
    void clear();

    void update(float deltaTime);
protected:
    std::unique_ptr<class StateMachineData> data;
};
