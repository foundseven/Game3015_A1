#include "Entity.hpp"

/**
 * @brief Constructor for Entity.
 * @param game Pointer to the Game object.
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
 * @brief Updates the current state of the entity.
 *
 * This method calculates the displacement based on the entity's velocity and the time elapsed,
 * moves the entity accordingly, updates the world transform, and marks the renderer as dirty.
 *
 * @param gt const reference to GameTimer object.
 */
//using our game timer
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

//adding definition of accelerate
void Entity::accelerate(XMFLOAT3 velocity)
{
	mVelocity.x = mVelocity.x + velocity.x;
	mVelocity.y = mVelocity.y + velocity.y;
	mVelocity.z = mVelocity.z + velocity.z;
}

void Entity::accelerate(float vx, float vy, float vz)
{
	mVelocity.x = mVelocity.x + vx;
	mVelocity.y = mVelocity.y + vy;
	mVelocity.z = mVelocity.z + vz;
}
