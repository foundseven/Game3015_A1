#pragma once
#pragma region step 7
#include "State.hpp"
#include "../../Common/GameTimer.h"
#include <memory>

/**
 * @brief Title screen state handling introductory visuals and transitions
 *
 * Manages title screen presentation including:
 * - Animated title graphics
 * - "Press Start" prompt flashing
 * - Transition to main menu
 */
class TitleState : public State
{
public:
    /**
     * @brief Constructs a TitleState with stack and context
     * @param stack State stack for transition management
     * @param context Shared context containing game resources
     */
    TitleState(StateStack* stack, Context* context);

    /**
     * @brief Destructor for resource cleanup
     */
    virtual ~TitleState();

    //-------------------------------------------------------------------------
    // State Interface Implementation
    //-------------------------------------------------------------------------

    /**
     * @brief Renders title screen elements
     * @override Required State override
     */
    virtual void Draw() override;

    /**
     * @brief Updates title screen animations and logic
     * @param gt Game timer with frame timing information
     * @return True to continue updates, false to block lower states
     * @override Required State override
     */
    virtual bool Update(const GameTimer& gt) override;

    /**
     * @brief Handles discrete input events (e.g., key presses)
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

private:
    //-------------------------------------------------------------------------
    // Title Screen Animation
    //-------------------------------------------------------------------------

    float mFlashTimer;     ///< Timer for visual element flashing
    bool mIsVisible;       ///< Current visibility state for flashing elements
};

#pragma endregion