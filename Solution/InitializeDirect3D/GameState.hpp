#pragma once
#include "State.hpp"
#include "World.hpp"
#include "Player.hpp"

class GameState : public State
{
public:
    GameState(StateStack* stack, Context* context);
    virtual ~GameState();

    virtual void Draw()override;

    virtual bool Update(const GameTimer& gt)override;

    virtual bool HandleEvent(WPARAM btnState)override;

    virtual bool HandleRealTimeInput()override;

    void ProcessInput();
private:
    World mWorld;

public:
    std::unique_ptr<SceneNode> mPauseStateSceneGraph;
};

