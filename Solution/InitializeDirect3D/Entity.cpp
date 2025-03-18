#include "Entity.hpp"

/**
 * @brief Constructs an Entity object
 * @param game Pointer to the parent Game object
 *
 * Initializes the Entity with zero velocity and sets up the SceneNode hierarchy.
 */
Entity::Entity(Game* game) : SceneNode(game), mVelocity(0, 0, 0)
{
}

/**
 * @brief Sets the velocity of the entity.
 * @param velocity XMFLOAT3 representing the new velocity.
 */
void Entity::setVelocity(XMFLOAT3 velocity)
{
	mVelocity = velocity;
}

/**
 * @brief Sets the velocity of the entity using individual components.
 * @param vx Velocity in the x-direction.
 * @param vy Velocity in the y-direction.
 * @param vz Velocity in the z-direction.
 */
void Entity::setVelocity(float vx, float vy, float vz)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
	mVelocity.z = vz;
}

/**
 * @brief Gets the current velocity of the entity.
 * @return XMFLOAT3 representing the current velocity.
 */
XMFLOAT3 Entity::getVelocity() const
{
	return mVelocity;
}

/**
 * @brief Updates the entity's position based on its velocity
 * @param gt Reference to GameTimer for frame timing
 *
 * This method:
 * 1. Calculates displacement based on velocity and elapsed time
 * 2. Updates the entity's position
 * 3. Updates the world transform
 * 4. Marks the renderer as dirty for the next frame
 */
void Entity::updateCurrent(const GameTimer& gt) 
{
	XMFLOAT3 mV;
	mV.x = mVelocity.x * gt.DeltaTime();
	mV.y = mVelocity.y * gt.DeltaTime();
	mV.z = mVelocity.z * gt.DeltaTime();

	move(mV.x, mV.y, mV.z);

	renderer->World = getWorldTransform();
	renderer->NumFramesDirty++;
}

/**
 * @brief Applies an acceleration to the entity's velocity
 * @param velocity Acceleration vector to apply
 *
 * Adds the given velocity vector to the current velocity.
 */
void Entity::accelerate(XMFLOAT3 velocity)
{
	mVelocity.x = mVelocity.x + velocity.x;
	mVelocity.y = mVelocity.y + velocity.y;
	mVelocity.z = mVelocity.z + velocity.z;
}

/**
 * @brief Applies an acceleration to the entity's velocity using components
 * @param vx Acceleration in the x-direction
 * @param vy Acceleration in the y-direction
 * @param vz Acceleration in the z-direction
 *
 * Adds the given velocity components to the current velocity.
 */
void Entity::accelerate(float vx, float vy, float vz)
{
	mVelocity.x = mVelocity.x + vx;
	mVelocity.y = mVelocity.y + vy;
	mVelocity.z = mVelocity.z + vz;
}
