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

}

void Player::HandleEvent(CommandQueue& commands)
{

}

void Player::HandeRealTimeInput(CommandQueue& commands)
{

}

#pragma endregion