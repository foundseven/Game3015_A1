#include "PauseState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"
#include <windows.h>

PauseState::PauseState(StateStack* stack, Context* context)
    : State(stack, context)
{
    if (!mSceneGraph)
    {
        OutputDebugStringA("Error: mSceneGraph is nullptr!\n");
    }
    else
    {
        OutputDebugStringA("mSceneGraph initialized.\n");
    }
    mAllRitems.clear();
    mContext->game->ResetFrameResources();
    mContext->game->BuildMaterials();

    std::unique_ptr<SpriteNode> backgroundSprite = std::make_unique<SpriteNode>(this);
    backgroundSprite->SetDrawName("Galaxy", "boxGeo", "box");
    backgroundSprite->setScale(1.0, 1.0, 1.0);
    backgroundSprite->setPosition(0, 0, 0);
    mSceneGraph->attachChild(std::move(backgroundSprite));

    mSceneGraph->build();
    mContext->game->BuildFrameResources(mAllRitems.size());
}

PauseState::~PauseState()
{
}

void PauseState::Draw()
{
    mSceneGraph->draw();
}

bool PauseState::Update(const GameTimer& gt)
{
    mSceneGraph->update(gt);
    PauseState::CheckPause(gt);
    return true;
}

bool PauseState::HandleEvent(WPARAM btnState)
{

    return true;
}


bool PauseState::HandleRealTimeInput()
{
    return true;
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