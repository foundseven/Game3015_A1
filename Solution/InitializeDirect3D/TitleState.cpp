#include "TitleState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"
#include <windows.h>

/**
 * @brief Constructs the Title State with core resources
 * @param stack State stack for transition management
 * @param context Shared context containing game resources
 *
 * Initializes:
 * - Scene graph hierarchy
 * - Sprite elements (background, title, planet, star)
 * - Frame resources
 * - Flashing animation system
 */
TitleState::TitleState(StateStack* stack, Context* context)
    : State(stack, context)
    , mFlashTimer(0.0f)
    , mIsVisible(true)
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

    // Title Text Sprite
    std::unique_ptr<SpriteNode> TSSprite = std::make_unique<SpriteNode>(this);
    TSSprite->SetDrawName("Title", "boxGeo", "box");
    TSSprite->setScale(5.0, 1.0, 2.0);
    TSSprite->setPosition(0, 1, -2);
    mSceneGraph->attachChild(std::move(TSSprite));

    // Planetary Decoration Sprite
    std::unique_ptr<SpriteNode> PlanetSprite = std::make_unique<SpriteNode>(this);
    PlanetSprite->SetDrawName("PlanetOne", "boxGeo", "box");
    PlanetSprite->setScale(2.0, 1.0, 2.0);
    PlanetSprite->setPosition(2, 1, 1);
    mSceneGraph->attachChild(std::move(PlanetSprite));

    // Animated Star Sprite
    std::unique_ptr<SpriteNode> StarSprite = std::make_unique<SpriteNode>(this);
    StarSprite->SetDrawName("Star", "boxGeo", "box");
    StarSprite->setScale(.75, .75, .75);
    StarSprite->setPosition(1.2, 1.5, 1.5);
    mSceneGraph->attachChild(std::move(StarSprite));

    // Finalize scene setup
    mSceneGraph->build();
    mContext->game->BuildFrameResources(mAllRitems.size());
}

/**
 * @brief Destructor for resource cleanup
 */
TitleState::~TitleState()
{
}

/**
 * @brief Renders all visible scene elements
 */
void TitleState::Draw()
{
    OutputDebugStringA("Drawing title frame...\n");
    mSceneGraph->draw();
}

/**
 * @brief Updates state logic and animations
 * @param gt Game timer with frame timing
 * @return True to continue updates, false to block lower states
 */
bool TitleState::Update(const GameTimer& gt)
{
    mSceneGraph->update(gt);

    //-------------------------------------------------------------------------
    // Flashing Title Animation
    //-------------------------------------------------------------------------
    mFlashTimer += gt.DeltaTime();

    // Update title visibility
    if (mFlashTimer >= 0.5f)
    {
        mFlashTimer = 0.0f;  // Reset the timer
        mIsVisible = !mIsVisible;  // Toggle visibility
    }

    for (auto& node : mSceneGraph->getChildren())
    {
        if (auto spriteNode = dynamic_cast<SpriteNode*>(node.get()))
        {
            if (spriteNode->GetDrawName() == "Title")
            {
                // Set visibility for the sprite node
                spriteNode->SetVisible(mIsVisible);
                break;  // Stop once the "Title" node is found and updated
            }
        }
        else
        {
            OutputDebugStringA("Could not cast...\n");
        }
    }
    return true;
}

/**
 * @brief Handles discrete input events
 * @param btnState Bitmask of button states
 * @return True if event was consumed, false to propagate
 */
bool TitleState::HandleEvent(WPARAM btnState)
{
    // If any key is pressed, trigger the next screen
    RequestStackPop();
    OutputDebugStringA("Moving to Menu...\n");

    RequestStackPush(States::Menu);

    return true;
}

/**
 * @brief Handles continuous input processing
 * @return True if input was processed, false to propagate
 */
bool TitleState::HandleRealTimeInput()
{
    return true;
}