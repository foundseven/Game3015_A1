#pragma region Step 17
#pragma once
#include "CommandQueue.hpp"
#include <map>	

//set this up for the player
class Player
{
public:
						Player();
	void				HandleEvent(CommandQueue& commands);
	void				HandeRealTimeInput(CommandQueue& commands);

};

#pragma endregion

