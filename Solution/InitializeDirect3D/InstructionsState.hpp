#pragma once
#include "State.hpp"
#include "SpriteNode.h"

/**
 * @brief State for displaying game instructions/controls
 *
 * Handles presentation of tutorial information and control schemes.
 * Inherits core state functionality while specializing for instructional content.
 */
class InstructionsState : public State
{
public:
    /**
     * @brief Constructs an InstructionsState with stack and context
     * @param stack Reference to state stack for push/pop operations
     * @param context Shared game context (resources, settings, etc.)
     */
    InstructionsState(StateStack* stack, Context* context);

    /**
     * @brief Destructor for cleanup operations
     */
    virtual ~InstructionsState();

    //-------------------------------------------------------------------------
    // State Interface Implementation
    //-------------------------------------------------------------------------

    /**
     * @brief Renders instructional content and UI elements
     * @override Required State override
     */
    virtual void Draw() override;

    /**
     * @brief Updates instructional content animations/timing
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
};