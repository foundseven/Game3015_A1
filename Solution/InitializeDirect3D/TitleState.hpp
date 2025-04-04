#pragma once
#pragma region step 7
#include "State.hpp"
#include "../../Common/GameTimer.h"
#include <memory>

class TitleState : public State
{
public:
	TitleState(StateStack* stack, Context* context);
    virtual ~TitleState();

    virtual void                Draw()override;
    virtual bool                Update(const GameTimer& gt)override;
    virtual bool                HandleEvent(WPARAM btnState)override;
    virtual bool                HandleRealTimeInput()override;

private:
    float                       mFlashTimer;
    bool                        mIsVisible;
};

#pragma endregion