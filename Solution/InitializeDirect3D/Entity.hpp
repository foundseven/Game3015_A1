#pragma once
#include "SceneNode.hpp"

/**
 * @brief Represents an entity in the game world, inheriting from SceneNode.
 */
class Entity :
    public SceneNode
{
public:
	/**
	 * @brief Constructor for Entity.
	 * @param game Pointer to the Game object.
	 */
	Entity(Game* game);

	/**
	 * @brief Sets the velocity of the entity.
	 * @param velocity XMFLOAT3 representing the new velocity.
	 */
	void				setVelocity(XMFLOAT3 velocity);
	/**
	 * @brief Sets the velocity of the entity using individual components.
	 * @param vx Velocity in the x-direction.
	 * @param vy Velocity in the y-direction.
	 * @param vz Velocity in the z-direction.
	 */
	void				setVelocity(float vx, float vy, float vz);

	/**
	 * @brief Gets the velocity of the entity.
	 * @return XMFLOAT3 representing the velocity.
	 */
	XMFLOAT3			getVelocity() const;

#pragma region Step 4

	void				accelerate(XMFLOAT3 velocity);
	void				accelerate(float vx, float vy, float vz);

#pragma endregion


	/**
	 * @brief Updates the current state of the entity.
	 * @param gt const reference to GameTimer object.
	 */
	virtual	void		updateCurrent(const GameTimer& gt);

public:
	XMFLOAT3		mVelocity; ///< The velocity of the entity.

};

