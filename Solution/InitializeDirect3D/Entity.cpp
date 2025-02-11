#include "Entity.hpp"

//Entity::Entity(Game* game) : SceneNode(game), mVelocity(0, 0)
//{
//}

Entity::Entity(Game* game) : SceneNode(game), mVelocity(0, 0, 0)
{
}

//two set velocity?
//void Entity::setVelocity(XMFLOAT2 velocity)
//{
//	mVelocity = velocity;
//}
//change this to enable set velocity
//void Entity::setVelocity(float vx, float vy)
//{
//	mVelocity.x = vx;
//	mVelocity.y = vy;
//}
void Entity::setVelocity(XMFLOAT3 velocity)
{
	mVelocity = velocity;
}
void Entity::setVelocity(float vx, float vy, float vz)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
	mVelocity.z = vz;
}

//XMFLOAT2 Entity::getVelocity() const
//{
//	return mVelocity;
//}
XMFLOAT3 Entity::getVelocity() const
{
	return mVelocity;
}
void Entity::updateCurrent(const GameTimer& gt) 
{
	/*XMFLOAT2 mV;
	mV.x = mVelocity.x * gt.DeltaTime();
	mV.y = mVelocity.y * gt.DeltaTime();

	move(mV.x, mV.y, 0);

	renderer->World = getWorldTransform();
	renderer->NumFramesDirty++;*/

	XMFLOAT3 mV;
	mV.x = mVelocity.x * gt.DeltaTime();
	mV.y = mVelocity.y * gt.DeltaTime();
	mV.z = mVelocity.z * gt.DeltaTime();

	move(mV.x, mV.y, mV.z);

	renderer->World = getWorldTransform();
	renderer->NumFramesDirty++;
}
