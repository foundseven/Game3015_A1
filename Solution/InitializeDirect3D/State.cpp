#include "State.hpp"
#include "Game.hpp"
#include <iostream>

/**
 * @brief Constructs a Context object with game and player references
 * @param _game Pointer to main Game instance
 * @param _player Pointer to player entity
 *
 * @note The Context object becomes invalid if Game/Player pointers are destroyed
 */
State::Context::Context(Game* _game, Player* _player)
    : game(_game)
    , player(_player)
{
}

/**
 * @brief Constructs a State with stack and context
 * @param stack State stack manager
 * @param context Shared context object
 *
 * Initializes:
 * - State stack reference
 * - Shared context
 * - Empty scene graph
 */
State::State(StateStack* stack, Context* context)
    : mStack(stack)
    , mContext(context)
    , mSceneGraph(std::make_unique<SceneNode>(this))
{
}

/**
 * @brief Destructor for state cleanup
 *
 * @note Automatically cleans up scene graph and render items
 */
State::~State()
{
}

/**
 * @brief Requests a state push operation
 * @param stateID Identifier of state to push
 *
 * @note Operation is deferred until safe stack modification
 */
void State::RequestStackPush(States::ID stateID)
{
    mStack->pushState(stateID);
}

/**
 * @brief Requests a state pop operation
 *
 * Removes current state from stack
 * @note Operation is deferred until safe stack modification
 */
void State::RequestStackPop()
{
    mStack->popState();
}

/**
 * @brief Requests stack clearance
 *
 * Removes all states from stack
 * @note Operation is deferred until safe stack modification
 */
void State::RequestStateClear()
{
    mStack->clearStates();
}

/**
 * @brief Gets shared context object
 * @return Pointer to context instance
 *
 * @warning Returns nullptr if context was destroyed
 */
State::Context* State::GetContext() const
{
    return mContext;
}