#pragma once
#include "State.hpp"
#include "../../Common/d3dApp.h"

#include <vector>
#include <utility>
#include <functional>
#include <map>

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

    void                        pushState(States::ID stateID);
    void                        popState();
    void                        clearStates();

    bool                        isEmpty() const { return mStack.empty(); }

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

