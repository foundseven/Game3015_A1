#include "TitleState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"
#include <windows.h>


TitleState::TitleState(StateStack* stack, Context* context)
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
    backgroundSprite->setScale(10.0, 1.0, 7.0);
    backgroundSprite->setPosition(0, 0, 0);
    mSceneGraph->attachChild(std::move(backgroundSprite));

    mContext->game->CreateText(L"TESTING");
    mSceneGraph->build();
    mContext->game->BuildFrameResources(mAllRitems.size());
}

TitleState::~TitleState()
{
}

void TitleState::Draw()
{
    OutputDebugStringA("Drawing title frame...\n");
    mSceneGraph->draw();

    mContext->game->DrawTheText();
}

bool TitleState::Update(const GameTimer& gt)
{
    mSceneGraph->update(gt);

    return true;
}

bool TitleState::HandleEvent(WPARAM btnState)
{
    // If any key is pressed, trigger the next screen
    RequestStackPop();
    OutputDebugStringA("Moving to game...\n");

    RequestStackPush(States::Game);

    return true;
}

bool TitleState::HandleRealTimeInput()
{
    return true;
}

