#pragma once
#include "State.hpp"


class MainMenuState : public State
{
public:
	MainMenuState(StateStack* stack, Context* context);
	virtual ~MainMenuState();

	virtual void                Draw()override;
	virtual bool                Update(const GameTimer& gt)override;
	virtual bool                HandleEvent(WPARAM btnState)override;
	virtual bool                HandleRealTimeInput()override;

};

