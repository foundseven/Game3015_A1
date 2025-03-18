#pragma region Step 10
#pragma once

#include "Command.hpp"
#include <queue>

/**
 * @class CommandQueue
 * @brief Thread-unsafe FIFO container for game commands
 *
 * Manages a queue of Command objects for deferred execution. Typically used to:
 * - Buffer input events between frames
 * - Queue gameplay actions for later processing
 * - Implement command patterns for undo/redo systems
 *
 * @note This is a temporary implementation for Step 10 - consider adding:
 *       - Thread safety (mutexes)
 *       - Priority system
 *       - Capacity limits
 */
class CommandQueue
{
public:
    /**
    * @brief Adds a command to the end of the queue
    * @param command Command to enqueue (copied)
    *
    * @warning Command objects should be lightweight. For heavy payloads,
    *          consider using shared pointers or move semantics.
    */
    void                push(const Command& command);

    /**
    * @brief Removes and returns the front command
    * @return Command from front of queue
    *
    * @pre !isEmpty() - undefined behavior if queue is empty
    * @post Command is removed from queue
    *
    * @note Use in conjunction with isEmpty() for safe consumption:
    * @code
    * while (!queue.isEmpty()) {
    *     auto cmd = queue.pop();
    *     // process command
    * }
    * @endcode
    */
    Command             pop();

    /**
    * @brief Checks if queue contains any commands
    * @return true if queue is empty, false otherwise
    *
    * @note Safe to call from any thread (const operation)
    */
    bool                isEmpty() const;

private:
    std::queue<Command> mQueue; ///< Underlying command storage (FIFO order)
};

#pragma endregion
