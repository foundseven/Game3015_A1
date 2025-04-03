#pragma once
#include "State.hpp"

#include <vector>
#include <utility>
#include <functional>
#include <map>

class Game;

class StateStack
{
public:
    enum Action
    {
        Push,
        Pop,
        Clear
    };

public:
    explicit                    StateStack(State::Context context);

    template <typename T>
    void                        registerState(States::ID stateID);

    void                        Update(const GameTimer& timer);
    void                        Draw();
    void                        HandleEvent(WPARAM btnState);
    void                        handleRealTimeInput();

    void                        pushState(States::ID stateID);
    void                        popState();
    void                        clearStates();

    bool                        isEmpty() const;

    State*                      GetCurrentState();
    State*                      GetPreviousState();

private:
    State::StatePtr             createState(States::ID stateID);
    void                        applyPendingChanges();

private:
    struct PendingChange
    {
        explicit                PendingChange(Action action, States::ID stateID = States::None);

        Action                  action;
        States::ID              stateID;
    };

private:
    std::vector<State::StatePtr> mStack;
    std::vector<PendingChange>  mPendingList;
    State::Context              mContext;
    std::map<States::ID, std::function<State::StatePtr()>> mFactories;
};

template <typename T>
void StateStack::registerState(States::ID stateID)
{
    mFactories[stateID] = [this]()
        {
            return State::StatePtr(new T(this, &mContext));
        };
}
