#pragma region Step 11
#include "CommandQueue.hpp"
#include "SceneNode.hpp"

/**
 * @class CommandQueue
 * @brief A FIFO queue for managing game commands
 *
 * The CommandQueue class is part of the command pattern implementation.
 * It stores commands that can be processed sequentially and applied to
 * scene nodes in the game world.
 */

 /**
  * @brief Pushes a command onto the queue
  * @param command The Command object to be added to the queue
  *
  * Adds a new command to the back of the queue for later processing.
  */
void CommandQueue::push(const Command& command)
{
    mQueue.push(command);
}

/**
 * @brief Removes and retrieves the front command from the queue
 * @return The Command object at the front of the queue
 *
 * Removes the front command from the queue and returns it. This method assumes
 * that the queue is not empty. Use `isEmpty()` to check before calling this method.
 *
 * @pre The queue must not be empty.
 * @post The command at the front of the queue is removed.
 */
Command CommandQueue::pop()
{
    Command command = mQueue.front();
    mQueue.pop();
    return command;
}

/**
 * @brief Checks if the queue is empty
 * @return true if the queue is empty, false otherwise
 *
 * This method allows you to safely check whether there are any commands left in
 * the queue before attempting to pop a command.
 */
bool CommandQueue::isEmpty() const
{
    return mQueue.empty();
}

#pragma endregion
