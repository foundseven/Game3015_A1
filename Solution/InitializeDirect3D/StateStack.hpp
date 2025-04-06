#pragma once
#include "State.hpp"
#include <vector>
#include <utility>
#include <functional>
#include <map>

class Game;

/**
 * @brief State stack manager for handling game state transitions
 *
 * Manages a stack of game states with LIFO semantics, providing:
 * - State transitions (push/pop/clear)
 * - Update/draw propagation
 * - Input event routing
 * - State factory registration
 */
class StateStack
{
public:
    /**
     * @brief Enumeration of state stack operations
     */
    enum Action
    {
        Push,  ///< Add new state to stack
        Pop,   ///< Remove top state from stack
        Clear  ///< Clear all states from stack
    };

public:
    /**
     * @brief Constructs a StateStack with shared context
     * @param context Shared resources for created states
     */
    explicit StateStack(State::Context context);

    /**
     * @brief Registers a state type with the stack factory
     * @tparam T Concrete state type to register
     * @param stateID State identifier from States::ID
     */
    template <typename T>
    void registerState(States::ID stateID);

    //-------------------------------------------------------------------------
    // State Stack Operations
    //-------------------------------------------------------------------------

    /**
     * @brief Updates all active states from top to bottom
     * @param timer Game timer with frame timing information
     */
    void Update(const GameTimer& timer);

    /**
     * @brief Draws all active states from bottom to top
     */
    void Draw();

    /**
     * @brief Routes input events through the state stack
     * @param btnState Bitmask of button states
     */
    void HandleEvent(WPARAM btnState);

    /**
     * @brief Handles real-time input processing
     */
    void handleRealTimeInput();

    //-------------------------------------------------------------------------
    // State Management
    //-------------------------------------------------------------------------

    /**
     * @brief Requests a state push operation
     * @param stateID Identifier of state to push
     */
    void pushState(States::ID stateID);

    /**
     * @brief Requests a state pop operation
     */
    void popState();

    /**
     * @brief Requests stack clearance
     */
    void clearStates();

    //-------------------------------------------------------------------------
    // State Queries
    //-------------------------------------------------------------------------

    /**
     * @brief Checks if stack is empty
     * @return True if no states are active
     */
    bool isEmpty() const;

    /**
     * @brief Gets current topmost state
     * @return Pointer to active state or nullptr if empty
     */
    State* GetCurrentState();

    /**
     * @brief Gets previous state (second from top)
     * @return Pointer to previous state or nullptr if unavailable
     */
    State* GetPreviousState();

private:
    //-------------------------------------------------------------------------
    // Internal Operations
    //-------------------------------------------------------------------------

    /**
     * @brief Creates a new state instance
     * @param stateID Identifier of state to create
     * @return Unique pointer to new state instance
     */
    State::StatePtr createState(States::ID stateID);

    /**
     * @brief Applies pending stack operations
     * @note Called at safe times to prevent mid-frame state changes
     */
    void applyPendingChanges();

private:
    /**
     * @brief Pending stack operation container
     */
    struct PendingChange
    {
        /**
         * @brief Constructs a pending change request
         * @param action Operation type
         * @param stateID Relevant state identifier (for Push operations)
         */
        explicit PendingChange(Action action, States::ID stateID = States::None);

        Action action;         ///< Operation type
        States::ID stateID;    ///< Target state identifier
    };

private:
    //-------------------------------------------------------------------------
    // Data Members
    //-------------------------------------------------------------------------

    std::vector<State::StatePtr> mStack;       ///< Active state stack
    std::vector<PendingChange> mPendingList;   ///< Pending operations
    State::Context mContext;                   ///< Shared state context

    /**
     * @brief State factory registry mapping IDs to creation functions
     */
    std::map<States::ID, std::function<State::StatePtr()>> mFactories;
};

// Template Implementation

/**
 * @brief Registers a state type with the stack
 * @tparam T Concrete state type (must inherit from State)
 * @param stateID Identifier to associate with this state type
 *
 * @note Example usage:
 * @code
 * stack.registerState<MenuState>(States::Menu);
 * @endcode
 */
template <typename T>
void StateStack::registerState(States::ID stateID)
{
    mFactories[stateID] = [this]()
        {
            return State::StatePtr(new T(this, &mContext));
        };
}