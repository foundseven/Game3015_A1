#pragma region Step 1
#pragma once

/**
 * @namespace Category
 * @brief Bitmask categories for scene object classification and collision filtering
 *
 * These categories are used to:
 * - Manage object interactions (collision detection)
 * - Filter rendering operations
 * - Organize scene graph queries
 *
 * @usage Combine using bitwise OR:
 * @code
 * Category::Type teamAircraft = Category::PlayerAircraft | Category::AlliedAircraft;
 * @endcode
 */
namespace Category
{
	/**
	* @enum Type
	* @brief Bitmask flags representing object categories in the game world
	*
	* Values represent distinct bit positions for efficient category combination
	* and testing using bitwise operations.
	*/
	enum Type
	{
		None = 0, ///< Default/no category (inactive objects)
		Scene = 1 << 0, ///< Static environment/level geometry
		PlayerAircraft = 1 << 1, ///< User-controlled aircraft (see Aircraft::Eagle)
		AlliedAircraft = 1 << 2, ///< Friendly NPC aircraft (non-player combatants)
		EnemyAircraft = 1 << 3, ///< Hostile aircraft (see Aircraft::Raptor)
	};
}

#pragma endregion