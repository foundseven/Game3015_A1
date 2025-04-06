#pragma once
#include "State.hpp"
#include "SpriteNode.h"

/**
 * @brief Main menu state handling title screen presentation and navigation
 *
 * Manages core menu functionality including:
 * - Animated menu elements
 * - Menu option selection
 * - Transition to gameplay states
 * - Visual effects (e.g., ship sprite animation)
 */
class MainMenuState : public State
{
public:
    /**
     * @brief Constructs a MainMenuState with stack and context
     * @param stack State stack for state management
     * @param context Shared game context (resources, input, etc.)
     */
    MainMenuState(StateStack* stack, Context* context);

    /**
     * @brief Destructor for resource cleanup
     */
    virtual ~MainMenuState();

    //-------------------------------------------------------------------------
    // State Interface Implementation
    //-------------------------------------------------------------------------

    /**
     * @brief Renders menu UI elements and animations
     * @override Required State override
     */
    virtual void Draw() override;

    /**
     * @brief Updates menu animations and logic
     * @param gt GameTimer with frame timing information
     * @return True to continue updates, false to block lower states
     * @override Required State override
     */
    virtual bool Update(const GameTimer& gt) override;

    /**
     * @brief Handles discrete input events (button presses)
     * @param btnState Bitmask of button states
     * @return True if event was consumed, false to propagate
     * @override Required State override
     */
    virtual bool HandleEvent(WPARAM btnState) override;

    /**
     * @brief Handles continuous input (keyboard/controller state)
     * @return True if input was processed, false to propagate
     * @override Required State override
     */
    virtual bool HandleRealTimeInput() override;

    //-------------------------------------------------------------------------
    // Menu Animation Properties
    //-------------------------------------------------------------------------

    SpriteNode* mShipMMSprite = nullptr;  ///< Pointer to animated ship sprite
    float mWiggleAngle = 0.0f;            ///< Current angle for wiggle animation
    float mWiggleSpeed = 2.0f;            ///< Wiggle animation speed (radians/sec)
    float mWiggleAmplitude = 10.0f;       ///< Maximum wiggle displacement (pixels)
};