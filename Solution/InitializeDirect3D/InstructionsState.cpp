#include "InstructionsState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"
#include <windows.h>

/**
 * @brief Constructs the Instructions State with UI elements
 * @param stack State stack manager
 * @param context Shared game context
 *
 * Initializes:
 * - Background galaxy texture
 * - Control scheme visualization (WASD)
 * - Return to menu button
 * - Frame rendering resources
 */
InstructionsState::InstructionsState(StateStack* stack, Context* context)
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

    // Control Scheme Visualization
    std::unique_ptr<SpriteNode> WASDSprite = std::make_unique<SpriteNode>(this);
    WASDSprite->SetDrawName("WASD", "boxGeo", "box");
    WASDSprite->setScale(3.5, 3.0, 3.0);
    WASDSprite->setPosition(0, 1, 0);
    mSceneGraph->attachChild(std::move(WASDSprite));

    // Return to Menu Button
    std::unique_ptr<SpriteNode> BackSprite = std::make_unique<SpriteNode>(this);
    BackSprite->SetDrawName("Back", "boxGeo", "box");
    BackSprite->setScale(3.0, 3.0, 3.0);
    BackSprite->setPosition(2.2, 1, -2.2);
    mSceneGraph->attachChild(std::move(BackSprite));

    // Finalize scene setup
    mSceneGraph->build();
    mContext->game->BuildFrameResources(mAllRitems.size());
}

/**
 * @brief Destructor for resource cleanup
 */
InstructionsState::~InstructionsState()
{
}

/**
 * @brief Renders instruction screen elements
 */
void InstructionsState::Draw()
{
    OutputDebugStringA("Drawing Instructions...\n");

    mSceneGraph->draw();
}

/**
 * @brief Updates instruction screen logic
 * @param gt Game timer with frame timing
 * @return True to continue updates in lower states
 */
bool InstructionsState::Update(const GameTimer& gt)
{
    mSceneGraph->update(gt);

    return true;
}

/**
 * @brief Handles input events for state transitions
 * @param btnState Bitmask of button states
 * @return True to consume event, false to propagate
 */
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

/**
 * @brief Handles real-time input processing
 * @return True to consume input, false to propagate
 */
bool InstructionsState::HandleRealTimeInput()
{
    return true;
}