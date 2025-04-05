#include "InstructionsState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"
#include <windows.h>

InstructionsState::InstructionsState(StateStack* stack, Context* context)
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

    std::unique_ptr<SpriteNode> WASDSprite = std::make_unique<SpriteNode>(this);
    WASDSprite->SetDrawName("WASD", "boxGeo", "box");
    WASDSprite->setScale(3.5, 3.0, 3.0);
    WASDSprite->setPosition(0, 1, 0);
    mSceneGraph->attachChild(std::move(WASDSprite));

    std::unique_ptr<SpriteNode> BackSprite = std::make_unique<SpriteNode>(this);
    BackSprite->SetDrawName("Back", "boxGeo", "box");
    BackSprite->setScale(3.0, 3.0, 3.0);
    BackSprite->setPosition(2.2, 1, -2.2);
    mSceneGraph->attachChild(std::move(BackSprite));

    mSceneGraph->build();
    mContext->game->BuildFrameResources(mAllRitems.size());
}

InstructionsState::~InstructionsState()
{
}

void InstructionsState::Draw()
{
    OutputDebugStringA("Drawing Instructions...\n");

    mSceneGraph->draw();
}

bool InstructionsState::Update(const GameTimer& gt)
{
    mSceneGraph->update(gt);

    return true;
}

bool InstructionsState::HandleEvent(WPARAM btnState)
{
    //input goes here
    if (d3dUtil::IsKeyDown('Q'))
    {
        RequestStackPop();
        RequestStackPush(States::Menu);

    }
    return true;
}

bool InstructionsState::HandleRealTimeInput()
{
    return true;
}