#pragma once
#include "State.hpp"

/**
 * @brief Game pause state handling paused game overlay and menu
 *
 * Manages pause functionality including:
 * - Rendering pause overlay/menu
 * - Handling pause-related input
 * - Resume/restart/quit options
 * - Time dilation effects
 */
class PauseState : public State
{
public:
    /**
     * @brief Constructs a PauseState with stack and context
     * @param stack State stack for state management
     * @param context Shared game context (resources, input, etc.)
     */
    PauseState(StateStack* stack, Context* context);

    /**
     * @brief Destructor for resource cleanup
     */
    virtual ~PauseState();

    //-------------------------------------------------------------------------
    // State Interface Implementation
    //-------------------------------------------------------------------------

    /**
     * @brief Renders pause menu/overlay elements
     * @override Required State override
     */
    virtual void Draw() override;

    /**
     * @brief Updates pause state logic
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
    // Custom Pause Functionality
    //-------------------------------------------------------------------------

    /**
     * @brief Visibility flag for pause overlay
     * @note When true, renders pause menu elements
     */
    bool showPause;

    /**
     * @brief Checks pause conditions and state transitions
     * @param gt GameTimer with frame timing information
     *
     * Handles:
     * - Pause toggle detection
     * - Time dilation effects
     * - State transition validation
     */
    void CheckPause(const GameTimer& gt);
};
