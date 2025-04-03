#include "State.hpp"
#include "Game.hpp"
#include <iostream>

State::Context::Context(Game* _game, Player* _player)
    : game(_game)
    , player(_player)
{
}

State::State(StateStack* stack, Context* context)
    : mStack(stack)
    , mContext(context)
    , mSceneGraph(std::make_unique<SceneNode>(this))
{
}

State::~State()
{
}

void State::RequestStackPush(States::ID stateID)
{
    mStack->pushState(stateID);
}

void State::RequestStackPop()
{
    mStack->popState();
}

void State::RequestStateClear()
{
    mStack->clearStates();
}

State::Context* State::GetContext() const
{
    return mContext;
}