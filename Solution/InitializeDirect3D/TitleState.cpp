#include "TitleState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"

TitleState::TitleState(StateStack* stack, Context* context)
    : State(stack, context)
{
    //render items here
    //background

    //text
}

void TitleState::Draw()
{
    //mSceneGraph->draw();
}

bool TitleState::Update(const GameTimer& gt)
{
    //mSceneGraph->update(gt);

    return true;
}

bool TitleState::HandleEvent(WPARAM btnState)
{
    // If any key is pressed, trigger the next screen
    RequestStackPop();
    //RequestStackPush(States::Menu);

    return true;
}

bool TitleState::HandleRealTimeInput()
{
    return true;
}