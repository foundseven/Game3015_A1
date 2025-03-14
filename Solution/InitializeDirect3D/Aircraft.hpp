#pragma once
#include "Entity.hpp"
#include <string>

/**
 * @brief Represents an aircraft in the game, inheriting from Entity.
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
		Eagle,
		Raptor,
	};


public:
	/**
	 * @brief Constructor for Aircraft.
	 * @param type The type of aircraft.
	 * @param game Pointer to the Game object.
	 */
	Aircraft(Type type, Game* game);

#pragma region Step 2
	virtual unsigned int getCategory() const;
#pragma endregion
private:
	/**
	 * @brief Draws the current aircraft.
	 */
	virtual void		drawCurrent() const;
	/**
	 * @brief Builds the current aircraft.
	 */
	virtual void		buildCurrent();


private:
	Type				mType; ///< The type of this aircraft.
	std::string			mSprite; ///< The sprite name for this aircraft.

	RenderItem* mAircraftRitem; ///< Pointer to the RenderItem for this aircraft.
};
