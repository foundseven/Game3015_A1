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

#pragma region Step 1 Part 2

	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		ActionCount
	};

	void AssignKey(Action action, char key);
	char getAssignedKey(Action action) const;

private:
	void InitializeActions();
	static bool isRealTimeAction(Action action);

private:
	std::map<char, Action>		mKeyBinding;
	std::map<Action, Command>	mActionBinding;

#pragma endregion

};

#pragma endregion

