#pragma once
#include "State.hpp"
#include "SpriteNode.h"

class InstructionsState : public State
{
public:
	InstructionsState(StateStack* stack, Context* context);
	virtual ~InstructionsState();

	virtual void                Draw()override;
	virtual bool                Update(const GameTimer& gt)override;
	virtual bool                HandleEvent(WPARAM btnState)override;
	virtual bool                HandleRealTimeInput()override;

};

