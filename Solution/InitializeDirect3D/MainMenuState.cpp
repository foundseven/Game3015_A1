#include "MainMenuState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"
#include <windows.h>

/**
 * @brief Constructs the Main Menu State with interactive elements
 * @param stack State stack manager
 * @param context Shared game context
 *
 * Initializes:
 * - Background galaxy texture
 * - Menu title text
 * - Animated ship decoration
 * - Frame rendering resources
 */
MainMenuState::MainMenuState(StateStack* stack, Context* context)
    : State(stack, context)
{
    // Debug validation
    if (!mSceneGraph)
    {
        OutputDebugStringA("Error: mSceneGraph is nullptr!\n");
    }
    else
    {
        OutputDebugStringA("mSceneGraph initialized.\n");
    }

    // Reset rendering resources
    mAllRitems.clear();
    mContext->game->ResetFrameResources();
    mContext->game->BuildMaterials();

    //-------------------------------------------------------------------------
    // Scene Graph Setup
    //-------------------------------------------------------------------------
    
    // Background Galaxy Sprite
    std::unique_ptr<SpriteNode> backgroundSprite = std::make_unique<SpriteNode>(this);
    backgroundSprite->SetDrawName("Galaxy", "boxGeo", "box");
    backgroundSprite->setScale(10.0, 1.0, 7.0);
    backgroundSprite->setPosition(0, 0, 0);
    mSceneGraph->attachChild(std::move(backgroundSprite));

    // Menu Title Text
    std::unique_ptr<SpriteNode> MMTextSprite = std::make_unique<SpriteNode>(this);
    MMTextSprite->SetDrawName("MMText", "boxGeo", "box");
    MMTextSprite->setScale(3, 1.0, 3);
    MMTextSprite->setPosition(2, 1, -1.5);
    mSceneGraph->attachChild(std::move(MMTextSprite));

    // Animated Ship Decoration
    std::unique_ptr<SpriteNode> ShipMMSprite = std::make_unique<SpriteNode>(this);
    ShipMMSprite->SetDrawName("ShipMM", "boxGeo", "box");
    ShipMMSprite->setScale(3.0, 3.0, 3.0);
    ShipMMSprite->setPosition(-2, 1, 0);
    mShipMMSprite = ShipMMSprite.get();
    mSceneGraph->attachChild(std::move(ShipMMSprite));

    // Finalize scene setup
    mSceneGraph->build();
    mContext->game->BuildFrameResources(mAllRitems.size());
}

/**
 * @brief Destructor for resource cleanup
 */
MainMenuState::~MainMenuState()
{
}

/**
 * @brief Renders menu elements
 */
void MainMenuState::Draw()
{
    OutputDebugStringA("Drawing menu...\n");

    mSceneGraph->draw();
}

/**
 * @brief Updates menu state logic and animations
 * @param gt Game timer with frame timing
 * @return True to continue updates in lower states
 */
bool MainMenuState::Update(const GameTimer& gt)
{
    mSceneGraph->update(gt);

    // Update ship wiggle animation
    if (mShipMMSprite)
    {
        mWiggleAngle += gt.DeltaTime() * XM_2PI * mWiggleSpeed;
        float angle = sinf(mWiggleAngle) * mWiggleAmplitude;
        mShipMMSprite->setWorldRotation(0.0f, XMConvertToRadians(angle), 0.0f);
    }
    return true;
}

/**
 * @brief Handles menu navigation input
 * @param btnState Bitmask of button states
 * @return True to consume event, false to propagate
 */
bool MainMenuState::HandleEvent(WPARAM btnState)
{
    // Start game on 'P'
    if (d3dUtil::IsKeyDown('P'))
    {
        RequestStackPop();
        RequestStackPush(States::Game);
    }
    // Show instructions on 'I'
    else if (d3dUtil::IsKeyDown('I'))
    {
        RequestStackPop();
        RequestStackPush(States::Instructions);
    }
    // Quit application on 'Q'
    else if (d3dUtil::IsKeyDown('Q'))
    {
        PostQuitMessage(0);
    }
    return true;
}

/**
 * @brief Handles real-time input processing
 * @return True to consume input, false to propagate
 */
bool MainMenuState::HandleRealTimeInput()
{
    return true;
}