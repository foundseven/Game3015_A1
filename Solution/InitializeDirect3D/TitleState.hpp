#pragma once
#pragma region step 7
#include "State.hpp"

class TitleState : public State
{
public:
	TitleState(StateStack* stack, Context* context);
    virtual void                Draw();
    virtual bool                Update(const GameTimer& gt);
    virtual bool                HandleEvent(WPARAM btnState);
    virtual bool                HandleRealTimeInput();
};

#pragma endregion