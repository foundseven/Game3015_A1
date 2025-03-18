#pragma region Step 5
#pragma once
#include "Category.hpp"
#include "../../Common/d3dApp.h"
#include <functional>
#include <cassert>

class SceneNode;

/**
 * @struct Command
 * @brief Encapsulates an executable action with category filtering
 *
 * Implements the Command pattern for game actions. Commands are executed
 * on SceneNodes that match the specified category mask.
 */
struct Command
{
    /**
    * @brief Constructs a default command with empty action and no category
    */
	Command();

    /**
    * @brief The action to execute, bound to a specific GameObject type
    * @param SceneNode& Target node to execute on
    * @param GameTimer& Reference to game timing information
    *
    * @note The function should be created using derivedAction() to ensure
    *       type safety and proper downcasting.
    */
	std::function<void(SceneNode&, const GameTimer&)> action;

    /**
    * @brief Bitmask of Category::Type flags specifying valid targets
    * @see Category::Type
    *
    * The command will only execute on nodes matching any of these categories.
    * Use bitwise OR to combine categories:
    * @code
    * Category::PlayerAircraft | Category::AlliedAircraft
    * @endcode
    */
	unsigned int category;
};

/**
 * @brief Creates type-safe command actions for specific GameObject types
 * @tparam GameObject Derived SceneNode type to target
 * @tparam Function Callable type with signature void(GameObject&, const GameTimer&)
 * @param fn Function to adapt
 * @return std::function<void(SceneNode&, const GameTimer&)> Wrapped function
 *
 * This function enables type-safe command dispatching by:
 * 1. Performing runtime type checking via dynamic_cast
 * 2. Providing type-safe static_cast to concrete GameObject type
 * 3. Generating compatible std::function for Command structs
 *
 * @example Create a move command for Aircraft:
 * @code
 * auto moveLeft = derivedAction<Aircraft>([](Aircraft& a, const GameTimer& gt) {
 *     a.accelerate(-gt.DeltaTime(), 0);
 * });
 * @endcode
 *
 * @warning The GameObject template parameter must be a concrete subclass of SceneNode
 * @note Uses assert() to verify correct type at runtime in debug builds
 */
template <typename GameObject, typename Function>
std::function<void(SceneNode&, const GameTimer&)> derivedAction(Function fn)
{
    return [=](SceneNode& node, const GameTimer& gameTimer)
        {
            // Runtime type safety check (debug builds only)
            assert(dynamic_cast<GameObject*>(&node) != nullptr);

            // Type-safe static dispatch
            fn(static_cast<GameObject&>(node), gameTimer);
        };
}
#pragma endregion