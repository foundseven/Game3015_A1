#pragma once
#include "../../Common/d3dApp.h"
#include "FrameResource.h"
#include "SceneNode.hpp"
#include <memory>

namespace sf
{
	class RenderWindow;
}

/**
 * @brief State management namespace containing state identifiers
 */
namespace States
{
    /**
     * @brief Enumeration of game state types
     */
    enum ID
    {
        None,           ///< Null/invalid state
        Title,          ///< Title screen state
        Menu,           ///< Main menu state
        Game,           ///< Core gameplay state
        Instructions,   ///< Tutorial/instructions state
        Pause           ///< Paused game state
    };
}

// Forward declarations
class StateStack;
class Player;
class Game;
class SceneNode;

/**
 * @brief Base class for all game states
 *
 * Provides core functionality for:
 * - State transition management
 * - Scene graph rendering
 * - Input handling
 * - Resource management
 */
class State
{
public:
    /// Unique pointer type for automatic state management
    typedef std::unique_ptr<State> StatePtr;

    /**
     * @brief Shared context for state-specific resources
     */
    struct Context
    {
        /**
         * @brief Constructs a context with game and player references
         * @param _game Pointer to main Game instance
         * @param _player Pointer to player entity
         */
        Context(Game* _game, Player* _player);

        Game* game;    ///< Main game instance containing shared resources
        Player* player;///< Player entity reference
    };

public:
    /**
     * @brief Constructs a State with stack and context
     * @param stack State stack manager
     * @param context Shared context object
     */
    State(StateStack* stack, Context* context);

    virtual ~State();

    //-------------------------------------------------------------------------
    // Core Interface
    //-------------------------------------------------------------------------

    /**
     * @brief Pure virtual method for state rendering
     */
    virtual void Draw() = 0;

    /**
     * @brief Pure virtual method for state updates
     * @param gt GameTimer with frame timing information
     * @return True to continue updates, false to block lower states
     */
    virtual bool Update(const GameTimer& gt) = 0;

    /**
     * @brief Pure virtual method for event handling
     * @param btnState Bitmask of button states
     * @return True if event was consumed, false to propagate
     */
    virtual bool HandleEvent(WPARAM btnState) = 0;

    /**
     * @brief Pure virtual method for real-time input handling
     * @return True if input was processed, false to propagate
     */
    virtual bool HandleRealTimeInput() = 0;

    //-------------------------------------------------------------------------
    // State Properties
    //-------------------------------------------------------------------------

    /**
     * @brief Checks if state allows lower states to render
     * @return True if state is transparent (default: false)
     */
    virtual bool IsTransparent() const { return false; }

    //-------------------------------------------------------------------------
    // Render Item Access
    //-------------------------------------------------------------------------

    /**
     * @brief Gets render items for drawing
     * @return Reference to vector of RenderItem pointers
     */
    std::vector<std::unique_ptr<RenderItem>>& getRenderItems() { return mAllRitems; }

    /**
     * @brief Gets shared context object
     * @return Pointer to context instance
     */
    Context* GetContext() const;

protected:
    //-------------------------------------------------------------------------
    // State Stack Operations
    //-------------------------------------------------------------------------

    /**
     * @brief Requests a new state to be pushed onto the stack
     * @param stateID ID of state to push
     */
    void RequestStackPush(States::ID stateID);

    /**
     * @brief Requests current state to be popped from stack
     */
    void RequestStackPop();

    /**
     * @brief Requests all states to be cleared from stack
     */
    void RequestStateClear();

protected:
    //-------------------------------------------------------------------------
    // Protected Members
    //-------------------------------------------------------------------------

    StateStack* mStack;       ///< State stack manager
    Context* mContext;        ///< Shared context reference

    std::unique_ptr<SceneNode> mSceneGraph; ///< Root scene node for state
    std::vector<std::unique_ptr<RenderItem>> mAllRitems; ///< Renderable items
};

