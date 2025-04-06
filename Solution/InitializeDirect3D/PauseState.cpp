#include "PauseState.hpp"
#include "SpriteNode.h"
#include "Game.hpp"
#include "GameState.hpp"
#include <windows.h>

/**
 * @brief Constructs the Pause State with core resources
 * @param stack State stack for transition management
 * @param context Shared context containing game resources
 *
 * Initializes pause state while maintaining reference to previous game state
 */
PauseState::PauseState(StateStack* stack, Context* context)
    : State(stack, context)
{

}

/**
 * @brief Destructor for resource cleanup
 */
PauseState::~PauseState()
{
}

/**
 * @brief Renders the paused game scene
 *
 * Draws the game state's preserved scene graph to maintain visual context
 */
void PauseState::Draw()
{
   // mSceneGraph->draw();
    //so i want to draw the specific from the previous state
    ((GameState*)(mStack->GetPreviousState()))->mPauseStateSceneGraph->draw();
}

/**
 * @brief Updates pause state logic
 * @param gt Game timer with frame timing
 * @return False to block updates in lower states
 */
bool PauseState::Update(const GameTimer& gt)
{
    //mSceneGraph->update(gt);
    //PauseState::CheckPause(gt);
    ((GameState*)(mStack->GetPreviousState()))->mPauseStateSceneGraph->update(gt);

    return false;
}

/**
 * @brief Handles pause-specific input events
 * @param btnState Bitmask of button states
 * @return False to propagate unhandled events
 */
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

/**
 * @brief Handles continuous input processing
 * @return False to propagate unprocessed input
 */
bool PauseState::HandleRealTimeInput()
{
    return false;
}

/**
 * @brief Checks for pause-related state transitions
 * @param gt Game timer with frame timing
 *
 * @note Currently contains redundant logic with HandleEvent()
 * @deprecated Consider removing in favor of unified input handling
 */
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