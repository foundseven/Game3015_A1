#pragma once
#include "State.hpp"
#include "SpriteNode.h"


class MainMenuState : public State
{
public:
	MainMenuState(StateStack* stack, Context* context);
	virtual ~MainMenuState();

	virtual void                Draw()override;
	virtual bool                Update(const GameTimer& gt)override;
	virtual bool                HandleEvent(WPARAM btnState)override;
	virtual bool                HandleRealTimeInput()override;

public:
	SpriteNode* mShipMMSprite = nullptr;
	float mWiggleAngle = 0.0f;
	float mWiggleSpeed = 2.0f; 
	float mWiggleAmplitude = 10.0f;

};

