#pragma region Step 17
#pragma once
#include "CommandQueue.hpp"
#include <map>	

/**
 * @class Player
 * @brief Handles player input and maps actions to commands
 *
 * The Player class manages key bindings and translates user input into
 * game commands that are pushed into a CommandQueue. It supports both
 * event-driven and real-time input handling.
 */
class Player
{
public:
	/**
	* @brief Constructs a Player object with default key bindings
	*/
						Player();

	/**
	 * @brief Handles event-driven input (e.g., key presses/releases)
	 * @param commands Reference to the CommandQueue for storing generated commands
	 */
	void				HandleEvent(CommandQueue& commands);

	/**
	* @brief Handles real-time input (e.g., continuous key holds)
	* @param commands Reference to the CommandQueue for storing generated commands
	*/
	void				HandeRealTimeInput(CommandQueue& commands);

#pragma region Step 1 Part 2

	/**
	 * @enum Action
	 * @brief Enumeration of player actions
	 *
	 * Actions represent high-level gameplay behaviors such as movement.
	 */
	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		ActionCount ///< Total number of actions (used for array sizing)
	};

	/**
	* @brief Assigns a key to a specific action
	* @param action The action to bind to a key
	* @param key The character key to bind to the action (e.g., 'W', 'A', 'S', 'D')
	*/
	void AssignKey(Action action, char key);

	/**
	* @brief Gets the key currently assigned to an action
	* @param action The action whose assigned key is requested
	* @return The character key bound to the specified action
	*/
	char getAssignedKey(Action action) const;

private:
	/**
	* @brief Initializes default actions and their corresponding commands
	*/
	void InitializeActions();

	/**
	* @brief Checks if an action is triggered by real-time input (continuous key hold)
	* @param action The action to check
	* @return true if the action is real-time, false otherwise
	*/
	static bool isRealTimeAction(Action action);

private:
	std::map<char, Action>		mKeyBinding; ///< Maps keys to actions (e.g., 'W' -> MoveUp)
	std::map<Action, Command>	mActionBinding; ///< Maps actions to their corresponding commands
	 
#pragma endregion

};

#pragma endregion

