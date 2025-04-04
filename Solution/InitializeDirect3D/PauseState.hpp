#pragma once
#include "State.hpp"

class PauseState : public State
{
public:
    PauseState(StateStack* stack, Context* context);
    virtual ~PauseState();

    virtual void                Draw()override;
    virtual bool                Update(const GameTimer& gt)override;
    virtual bool                HandleEvent(WPARAM btnState)override;
    virtual bool                HandleRealTimeInput()override;
    //virtual bool                IsTransparent()override;

    //i think i will need some different functions here to overlay
    //bool check?
    bool                        showPause;

    //function that checks to see if there was a button pressed
    void                        CheckPause(const GameTimer& gt);
};

