#pragma once
#include "SceneNode.hpp"

class Entity :
    public SceneNode
{
public:
	Entity(Game* game);
	//void				setVelocity(XMFLOAT2 velocity);
	void				setVelocity(XMFLOAT3 velocity);
	//void				setVelocity(float vx, float vy);
	void				setVelocity(float vx, float vy, float vz);
	//XMFLOAT2			getVelocity() const;
	XMFLOAT3			getVelocity() const;

	virtual	void		updateCurrent(const GameTimer& gt);

public:
	//XMFLOAT2		mVelocity;
	XMFLOAT3		mVelocity;

};

