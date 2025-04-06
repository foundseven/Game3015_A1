#pragma once
#include "State.hpp"
#include "World.hpp"
#include "Player.hpp"

/**
 * @brief Main gameplay state handling world simulation and player interaction
 *
 * Manages core game functionality including:
 * - World updates and rendering
 * - Player input handling (both events and real-time)
 * - Pause state management
 */
class GameState : public State
{
public:
    /**
     * @brief Constructs a GameState with state stack and context
     * @param stack State stack for push/pop operations
     * @param context Shared context containing game resources
     */
    GameState(StateStack* stack, Context* context);

    virtual ~GameState();

    //-------------------------------------------------------------------------
    // State Interface Implementation
    //-------------------------------------------------------------------------

    /**
     * @brief Draws all game world elements
     * @override Required State override
     */
    virtual void Draw() override;

    /**
     * @brief Updates game world logic
     * @param gt GameTimer with frame timing information
     * @return True to continue updates, false to block lower states
     * @override Required State override
     */
    virtual bool Update(const GameTimer& gt) override;

    /**
     * @brief Handles discrete input events
     * @param btnState Bitmask of button states
     * @return True if event was consumed, false to propagate
     * @override Required State override
     */
    virtual bool HandleEvent(WPARAM btnState) override;

    /**
     * @brief Handles continuous/real-time input
     * @return True if input was processed, false to propagate
     * @override Required State override
     */
    virtual bool HandleRealTimeInput() override;

    //-------------------------------------------------------------------------
    // Input Processing
    //-------------------------------------------------------------------------

    /**
     * @brief Centralized input processing method
     * @details Coordinates between event-driven and real-time input systems
     */
    void ProcessInput();

public:
    //-------------------------------------------------------------------------
    // Game World
    //-------------------------------------------------------------------------

    World mWorld;  ///< Main game world container managing entities and systems

    //-------------------------------------------------------------------------
    // Pause State Management
    //-------------------------------------------------------------------------

    std::unique_ptr<SceneNode> mPauseStateSceneGraph;  ///< Scene graph for pause menu overlay
};