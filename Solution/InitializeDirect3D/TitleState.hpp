#pragma once
#pragma region step 7
#include "State.hpp"

class TitleState : public State
{
public:
	TitleState(StateStack* stack, Context* context);
    virtual ~TitleState();

    virtual void                Draw();
    virtual bool                Update(const GameTimer& gt)override;
    virtual bool                HandleEvent(WPARAM btnState)override;
    virtual bool                HandleRealTimeInput()override;
};

#pragma endregion