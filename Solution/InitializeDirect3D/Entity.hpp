#pragma once
#include "SceneNode.hpp"

/**
 * @class Entity
 * @brief Represents a movable object in the game world
 * @extends SceneNode
 *
 * Entity adds velocity and movement capabilities to SceneNode objects.
 * It provides methods for setting and modifying velocity, as well as
 * updating the entity's position based on elapsed time.
 */
class Entity :
    public SceneNode
{
public:
	/**
	 * @brief Constructs an Entity with zero initial velocity
	 * @param game Pointer to the parent Game object
	 */
	Entity(State* state);

	/**
	 * @brief Sets the entity's velocity vector
	 * @param velocity New velocity as an XMFLOAT3
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
	 * @brief Retrieves the current velocity of the entity
	 * @return Current velocity as an XMFLOAT3
	 */
	XMFLOAT3			getVelocity() const;

#pragma region Step 4

	/**
	* @brief Applies an acceleration to the entity's velocity
	* @param velocity Acceleration vector to apply
	*/
	void				accelerate(XMFLOAT3 velocity);

	/**
	* @brief Applies an acceleration using individual components
	* @param vx Acceleration in the x-direction
	* @param vy Acceleration in the y-direction
	* @param vz Acceleration in the z-direction
	*/
	void				accelerate(float vx, float vy, float vz);

#pragma endregion


	/**
	 * @brief Updates the entity's position based on its velocity
	 * @param gt Reference to GameTimer for frame timing
	 * @override SceneNode::updateCurrent
	 */
	virtual	void		updateCurrent(const GameTimer& gt);

public:
	XMFLOAT3		mVelocity; ///< Current velocity vector of the entity

};

