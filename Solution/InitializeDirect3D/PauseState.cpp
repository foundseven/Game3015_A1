#include "PauseState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"
#include "GameState.hpp"

#include <windows.h>

PauseState::PauseState(StateStack* stack, Context* context)
    : State(stack, context)
{

}

PauseState::~PauseState()
{
}

void PauseState::Draw()
{
   // mSceneGraph->draw();
    //so i want to draw the specific from the previous state
    ((GameState*)(mStack->GetPreviousState()))->mPauseStateSceneGraph->draw();
}

bool PauseState::Update(const GameTimer& gt)
{
    //mSceneGraph->update(gt);
    //PauseState::CheckPause(gt);
    ((GameState*)(mStack->GetPreviousState()))->mPauseStateSceneGraph->update(gt);

    return false;
}

bool PauseState::HandleEvent(WPARAM btnState)
{
    if (d3dUtil::IsKeyDown('P'))
    {
        RequestStackPop();
    }
    if (d3dUtil::IsKeyDown('Q'))
    {
        //RequestStackPop();
        RequestStateClear();
        RequestStackPush(States::Menu);
    }
    return false;
}


bool PauseState::HandleRealTimeInput()
{
    return false;
}

void PauseState::CheckPause(const GameTimer& gt)
{
    if (d3dUtil::IsKeyDown('P'))
    {
        showPause = true;
        //RequestStackPop();
        //RequestStackPush(States::Game);
    }
    if (d3dUtil::IsKeyDown('Q'))
    {
        RequestStackPop();
        RequestStackPush(States::Menu);
    }
    if (d3dUtil::IsKeyDown(VK_BACK))
    {
        showPause = false;
       // RequestStackPop();
        //RequestStackPush(States::Game);
    }
    if (showPause)
    {
        //logic here?
    }
    if (!showPause)
    {

    }
}