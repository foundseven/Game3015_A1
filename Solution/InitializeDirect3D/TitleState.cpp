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

    std::unique_ptr<SpriteNode> TSSprite = std::make_unique<SpriteNode>(this);
    TSSprite->SetDrawName("Title", "boxGeo", "box");
    TSSprite->setScale(5.0, 1.0, 2.0);
    TSSprite->setPosition(0, 1, -2);
    mSceneGraph->attachChild(std::move(TSSprite));

    std::unique_ptr<SpriteNode> PlanetSprite = std::make_unique<SpriteNode>(this);
    PlanetSprite->SetDrawName("PlanetOne", "boxGeo", "box");
    PlanetSprite->setScale(2.0, 1.0, 2.0);
    PlanetSprite->setPosition(2, 1, 1);
    mSceneGraph->attachChild(std::move(PlanetSprite));

    std::unique_ptr<SpriteNode> StarSprite = std::make_unique<SpriteNode>(this);
    StarSprite->SetDrawName("Star", "boxGeo", "box");
    StarSprite->setScale(.75, .75, .75);
    StarSprite->setPosition(1.2, 1.5, 1.5);
    mSceneGraph->attachChild(std::move(StarSprite));

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
    OutputDebugStringA("Moving to Menu...\n");

    RequestStackPush(States::Menu);

    return true;
}

bool TitleState::HandleRealTimeInput()
{
    return true;
}

