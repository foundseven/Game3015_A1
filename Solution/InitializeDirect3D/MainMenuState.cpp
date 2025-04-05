#include "MainMenuState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"
#include <windows.h>

MainMenuState::MainMenuState(StateStack* stack, Context* context)
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

    std::unique_ptr<SpriteNode> MMTextSprite = std::make_unique<SpriteNode>(this);
    MMTextSprite->SetDrawName("MMText", "boxGeo", "box");
    MMTextSprite->setScale(3, 1.0, 3);
    MMTextSprite->setPosition(2, 1, -1.5);
    mSceneGraph->attachChild(std::move(MMTextSprite));

    std::unique_ptr<SpriteNode> ShipMMSprite = std::make_unique<SpriteNode>(this);
    ShipMMSprite->SetDrawName("ShipMM", "boxGeo", "box");
    ShipMMSprite->setScale(3.0, 3.0, 3.0);
    ShipMMSprite->setPosition(-2, 1, 0);
    mShipMMSprite = ShipMMSprite.get();
    mSceneGraph->attachChild(std::move(ShipMMSprite));

    //mContext->game->CreateText(L"TESTING");
    mSceneGraph->build();
    mContext->game->BuildFrameResources(mAllRitems.size());
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::Draw()
{
    OutputDebugStringA("Drawing menu...\n");

    mSceneGraph->draw();
}

bool MainMenuState::Update(const GameTimer& gt)
{
    mSceneGraph->update(gt);
    if (mShipMMSprite)
    {
        mWiggleAngle += gt.DeltaTime() * XM_2PI * mWiggleSpeed;
        float angle = sinf(mWiggleAngle) * mWiggleAmplitude; // degrees

        // Convert to radians for rotation method
        mShipMMSprite->setWorldRotation(0.0f, XMConvertToRadians(angle), 0.0f);
    }
    return true;
}

bool MainMenuState::HandleEvent(WPARAM btnState)
{
    //input goes here
    if (d3dUtil::IsKeyDown('P'))
    {
        RequestStackPop();
        RequestStackPush(States::Game);
    }
    else if (d3dUtil::IsKeyDown('I'))
    {
        RequestStackPop();
        RequestStackPush(States::Instructions);
    }
    else if (d3dUtil::IsKeyDown('Q'))
    {
        PostQuitMessage(0);
    }
    return true;
}

bool MainMenuState::HandleRealTimeInput()
{
    return true;
}