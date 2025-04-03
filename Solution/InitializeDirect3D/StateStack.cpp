#include "StateStack.hpp"
#pragma region step 6 - A3
StateStack::StateStack(State::Context context)
    : mStack()
    , mPendingList()
    , mContext(context)
    , mFactories()
{
}

void StateStack::Update(const GameTimer& timer)
{
    //Iterate from Top to Bottom, stop as soon as Update returns false
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->Update(timer))
            break;
    }

    applyPendingChanges();
}

void StateStack::Draw()
{
    //Draw all active states from Bottom to Top
    for (State::StatePtr& state : mStack)
    {
        state->Draw();
    }
}

void StateStack::HandleEvent(WPARAM btnState)
{
    //Iterate from Top to Bottom, stops as soon as HandleEvent() returns false
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->HandleEvent(btnState))
            break;
    }

    applyPendingChanges();
}

void StateStack::pushState(States::ID stateID)
{
    mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
    mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
    mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
    return mStack.empty();
}

State::StatePtr StateStack::createState(States::ID stateID)
{
    auto found = mFactories.find(stateID);
    assert(found != mFactories.end());

    return found->second();
}

void StateStack::applyPendingChanges()
{
    for (PendingChange change : mPendingList)
    {
        switch (change.action)
        {
        case Push:
            mStack.push_back(createState(change.stateID));
            break;
        case Pop:
            mStack.pop_back();
            break;
        case Clear:
            mStack.clear();
            break;
        }
    }

    mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
    : action(action),
    stateID(stateID)
{
}
#pragma endregion