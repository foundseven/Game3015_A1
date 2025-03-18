#pragma region Step 6
#include "Command.hpp"

/**
 * @class Command
 * @brief Encapsulates an action to be executed on scene nodes
 *
 * The Command class is part of the command pattern implementation,
 * allowing for flexible and decoupled execution of actions on scene nodes.
 */

 /**
  * @brief Default constructor for Command
  *
  * Initializes a Command object with no action and the None category.
  * This allows for later assignment of specific actions and categories.
  */
Command::Command()

	: action()
	, category(Category::None)
{
}

/**
 * @var Command::action
 * @brief Function object representing the action to be performed
 *
 * This std::function takes a SceneNode reference and a const GameTimer reference
 * as parameters, allowing for time-based actions on scene nodes.
 */

 /**
  * @var Command::category
  * @brief Category flags determining which scene nodes the command applies to
  *
  * Uses bit flags from the Category enum to specify target nodes.
  * Default value is Category::None.
  */
#pragma endregion
