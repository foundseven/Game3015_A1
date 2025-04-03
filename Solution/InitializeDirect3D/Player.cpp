#pragma region Step 18
#include "Player.hpp"
#include "CommandQueue.hpp"
#include "Aircraft.hpp"

#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#pragma region Step 2 - A3

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;
/**
 * @struct AircraftMover
 * @brief Functor for aircraft movement
 *
 * This struct encapsulates the logic for moving an aircraft based on a velocity vector.
 */
struct AircraftMover
{
    /**
    * @brief Constructor for AircraftMover
    * @param vx Velocity in x-direction
    * @param vy Velocity in y-direction
    * @param vz Velocity in z-direction
    */
    AircraftMover(float vx, float vy, float vz)
        : velocity(vx, vy, vz)
    {

    }

    /**
    * @brief Function call operator to apply movement to an aircraft
    * @param aircraft Reference to the Aircraft to move
    * @param gt GameTimer object (unused in this implementation)
    */
    void operator() (Aircraft& aircraft, const GameTimer&) const
    {
        aircraft.accelerate(velocity);
    }

    XMFLOAT3 velocity; ///< Velocity vector for aircraft movement
};

/**
 * @class Player
 * @brief Manages player input and actions
 *
 * The Player class handles key bindings, real-time input, and action execution for the player.
 */

 /**
  * @brief Constructor for Player
  *
  * Initializes key bindings and action bindings for the player.
  */
Player::Player()
{
    // Set key bindings
    mKeyBinding[VK_LEFT] = MoveLeft;
    mKeyBinding[VK_RIGHT] = MoveRight;
    mKeyBinding[VK_UP] = MoveUp;
    mKeyBinding[VK_DOWN] = MoveDown;

    mKeyBinding['A'] = MoveLeft;
    mKeyBinding['D'] = MoveRight;
    mKeyBinding['W'] = MoveUp;
    mKeyBinding['S'] = MoveDown;

    // Set action bindings
    InitializeActions();

    // Assign all categories to players aircraft
    for (auto& pair : mActionBinding)
    {
        pair.second.category = Category::PlayerAircraft;
    }
}

/**
 * @brief Handles non-real-time events
 * @param commands Reference to the CommandQueue to push commands to
 */
void Player::HandleEvent(CommandQueue& commands/*, WPARAM btnState*/)
{
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

/**
 * @brief Handles real-time input
 * @param commands Reference to the CommandQueue to push commands to
 */
void Player::HandeRealTimeInput(CommandQueue& commands)
{
    for (auto pair : mKeyBinding)
    {
        if (GetAsyncKeyState(pair.first) & 0x8000 && isRealTimeAction(pair.second))
        {
            commands.push(mActionBinding[pair.second]); 
        }
    }
}

/**
 * @brief Assigns a key to an action
 * @param action The action to assign
 * @param key The key to bind to the action
 */
void Player::AssignKey(Action action, char key)
{
    // Remove all keys that already map to this action
    for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end();)
    {
        if (itr->second == action)
            mKeyBinding.erase(itr++);
        else
            ++itr;
    }

    // Insert new binding
    mKeyBinding[key] = action;
}

/**
 * @brief Gets the key assigned to an action
 * @param action The action to look up
 * @return The key assigned to the action, or 0x00 if not found
 */
char Player::getAssignedKey(Action action) const
{
    for (auto pair : mKeyBinding)
    {
        if (pair.second == action)
            return pair.first;
    }

   // return 0x00;
    return 0;

}

/**
 * @brief Initializes action bindings
 */
void Player::InitializeActions()
{
    const float ps = 5.0f;

    mActionBinding[MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-ps, 0.0f, 0.0f));
    mActionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(+ps, 0.0f, 0.0f));
    mActionBinding[MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.0f, 0.0f, +ps));
    mActionBinding[MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.0f, 0.0f, -ps));
}

/**
 * @brief Checks if an action is real-time
 * @param action The action to check
 * @return true if the action is real-time, false otherwise
 */
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
#pragma endregion