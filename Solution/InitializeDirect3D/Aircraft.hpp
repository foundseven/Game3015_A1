#pragma once
#include "Entity.hpp"
#include <string>

/**
 * @brief Aircraft entity representing both player-controlled and AI-controlled planes
 * @ingroup entities
 *
 * Inherits from Entity and implements aircraft-specific rendering logic. Manages
 * both visual representation (through RenderItem) and collision category.
 */
class Aircraft :
    public Entity
{
public:
	/**
	 * @brief Enumeration of aircraft types.
	 */
	enum Type
	{
		Eagle, ///< Player-controlled aircraft
		Raptor, ///< AI-controlled enemy aircraft
	};


public:
	/**
	 * @brief Constructs a new Aircraft entity
	 * @param type Aircraft type from Type enumeration
	 * @param game Reference to parent Game instance
	 *
	 * @warning The Game object must outlive the Aircraft instance
	 */
	Aircraft(Type type, State* state);

#pragma region Step 2
	/**
	* @brief Gets collision category flags for this aircraft
	* @return unsigned int Combination of Category flags
	*
	* Overrides Entity::getCategory(). Return values:
	* - Category::PlayerAircraft for Eagle type
	* - Category::EnemyAircraft for all other types
	*/
	virtual unsigned int getCategory() const;
#pragma endregion

private:
	/**
	 * @brief Implements aircraft-specific drawing logic
	 *
	 * Overrides Entity::drawCurrent(). Configures DirectX 12 commands for:
	 * - Vertex/Index buffer setup
	 * - Constant buffer binding
	 * - Texture descriptor handling
	 * - Draw call submission
	 */
	virtual void		drawCurrent() const;

	/**
	 * @brief Builds render resources for the aircraft
	 *
	 * Overrides Entity::buildCurrent(). Creates and configures:
	 * - RenderItem instance
	 * - World transformation matrix
	 * - Material/geometry associations
	 * - CBV indices
	 */
	virtual void		buildCurrent();


private:
	Type				mType; ///< Aircraft type (determines behavior and rendering)
	std::string			mSprite; ///< Associated sprite resource name

	RenderItem* mAircraftRitem; ///< RenderItem used for frame resource management
};
