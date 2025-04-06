#include "StateStack.hpp"
#include <cassert>
#include "Game.hpp"

#pragma region step 6 - A3
/**
 * @brief Constructs a StateStack with shared context
 * @param context Shared resources for created states
 *
 * Initializes empty state stack and clean operation list
 */
StateStack::StateStack(State::Context context)
    : mStack()
    , mPendingList()
    , mContext(context)
    , mFactories()
{
}

/**
 * @brief Updates all active states from top to bottom
 * @param timer Game timer with frame timing information
 *
 * Stops propagation when a state returns false from Update()
 * Applies pending stack operations after updates
 */
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

/**
 * @brief Draws all active states from bottom to top
 *
 * Renders states in reverse order of updates to ensure proper layering
 */
void StateStack::Draw()
{
    //Draw all active states from Bottom to Top
    for (State::StatePtr& state : mStack)
    {
        state->Draw();
    }
}

/**
 * @brief Routes input events through the state stack
 * @param btnState Bitmask of button states
 *
 * Propagates events from top state downward until handled
 */
void StateStack::HandleEvent(WPARAM btnState)
{
    //Iterate from Top to Bottom, stops as soon as HandleEvent() returns false
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->HandleEvent(btnState))
            break;
    }
}

/**
 * @brief Handles real-time input processing
 *
 * Processes continuous input from top state downward until handled
 */
void StateStack::handleRealTimeInput()
{
    //Iterate from Top to Bottom, stop as soon as HandleEvent() returns false
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->HandleRealTimeInput())
            break;
    }
}

/**
 * @brief Requests a state push operation
 * @param stateID Identifier of state to push
 *
 * @note Actual push occurs during next applyPendingChanges()
 */
void StateStack::pushState(States::ID stateID)
{
    mPendingList.push_back(PendingChange(Push, stateID));
}

/**
 * @brief Requests a state pop operation
 *
 * @note Actual pop occurs during next applyPendingChanges()
 */
void StateStack::popState()
{
    mPendingList.push_back(PendingChange(Pop));
}

/**
 * @brief Requests stack clearance
 *
 * @note Actual clearance occurs during next applyPendingChanges()
 */
void StateStack::clearStates()
{
    mPendingList.push_back(PendingChange(Clear));
}

/**
 * @brief Checks if stack is empty
 * @return True if no states are active
 */
bool StateStack::isEmpty() const
{
    return mStack.empty();
}

/**
 * @brief Gets current topmost state
 * @return Pointer to active state (nullptr if empty)
 */
State* StateStack::GetCurrentState()
{
    return mStack.back().get();
}

/**
 * @brief Gets previous state (second from top)
 * @return Pointer to previous state (nullptr if <2 states)
 */
State* StateStack::GetPreviousState()
{
    if (mStack.size() >= 2)
    {
        return mStack[mStack.size() - 2].get();
    }
    return nullptr;  // Explicit null return for safety
}

/**
 * @brief Creates state instance using registered factory
 * @param stateID Identifier of state to create
 * @return Unique pointer to new state instance
 *
 * @throws std::runtime_error if stateID not registered
 */
State::StatePtr StateStack::createState(States::ID stateID)
{
    auto found = mFactories.find(stateID);
    assert(found != mFactories.end());

    return found->second();
}

/**
 * @brief Applies pending stack operations
 *
 * Processes operations in FIFO order:
 * 1. Pushes
 * 2. Pops
 * 3. Clears
 *
 * @note Called at safe times to prevent mid-frame state changes
 */
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

/**
 * @brief Constructs a pending stack operation
 * @param action Operation type (Push/Pop/Clear)
 * @param stateID Relevant state ID (for Push operations)
 */
StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
    : action(action)
    , stateID(stateID)
{
}

#pragma endregion