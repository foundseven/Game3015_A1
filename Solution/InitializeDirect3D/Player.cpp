#pragma region Step 18
#include "Player.hpp"
#include "CommandQueue.hpp"
#include "Aircraft.hpp"

#include <map>
#include <string>
#include <algorithm>
#include <iostream>

struct AircraftMover
{
    AircraftMover(float vx, float vy, float vz)
        : velocity(vx, vy, vz)
    {

    }

    void operator() (Aircraft& aircraft, const GameTimer&) const
    {
        aircraft.accelerate(velocity);
    }

    XMFLOAT3 velocity;
};

Player::Player()
{
    //set key bindings
    mKeyBinding[VK_LEFT] = MoveLeft;
    mKeyBinding[VK_RIGHT] = MoveRight;
    mKeyBinding[VK_UP] = MoveUp;
    mKeyBinding[VK_DOWN] = MoveDown;

    mKeyBinding['A'] = MoveLeft;
    mKeyBinding['D'] = MoveRight;
    mKeyBinding['W'] = MoveUp;
    mKeyBinding['S'] = MoveDown;

    //set action bindings
    InitializeActions();

    //assign all categories to players aircraft
    for (auto& pair : mActionBinding)
    {
        pair.second.category = Category::PlayerAircraft;
    }
}

void Player::HandleEvent(CommandQueue& commands)
{
   //need to figure out this one
    for (auto& pair : mKeyBinding)
    {
        if (!isRealTimeAction(pair.second))
        {
              if (GetAsyncKeyState(pair.first) & 0x8000)
              {
                  commands.push(mActionBinding[pair.second]);
              }
        }
    }

}

void Player::HandeRealTimeInput(CommandQueue& commands)
{
    for (auto pair : mKeyBinding)
    {
        if (GetAsyncKeyState(pair.first) & 0x8000 && isRealTimeAction(pair.second))
        {
            //holding Down
            commands.push(mActionBinding[pair.second]); 
        }
    }
}

void Player::AssignKey(Action action, char key)
{
    //remove all keys that already map action
        for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end();)
        {
            if (itr->second == action)
                mKeyBinding.erase(itr++);
            else
                ++itr;
        }

    //insert new binding
    mKeyBinding[key] = action;
}

char Player::getAssignedKey(Action action) const
{
    for (auto pair : mKeyBinding)
    {
        if (pair.second == action)
            return pair.first;
    }

    return 0x00;
}

void Player::InitializeActions()
{
    const float ps = 5.0f;

    mActionBinding[MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-ps, 0.0f, 0.0f));
    mActionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(+ps, 0.0f, 0.0f));
    //not working for some reason...?
    mActionBinding[MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.0f, 0.0f, +ps));
    mActionBinding[MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.0f, 0.0f, -ps));
}

bool Player::isRealTimeAction(Action action)
{
    switch (action)
    {
    case MoveLeft:
    case MoveRight:
    case MoveDown:
    case MoveUp:
        return true;

    default:
        return false;

    }
}
#pragma endregion