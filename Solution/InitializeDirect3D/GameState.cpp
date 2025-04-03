#include "GameState.hpp"
#include "Game.hpp"
#include <windows.h>

GameState::GameState(StateStack* stack, Context* context)
	: State(stack, context)
	, mWorld(this)
{
	mAllRitems.clear();
	mContext->game->ResetFrameResources();
	mContext->game->BuildMaterials();

	mWorld.buildScene();

	mContext->game->BuildFrameResources(mAllRitems.size());
}

GameState::~GameState()
{
}

void GameState::Draw()
{
	OutputDebugStringA("Drawing game frame...\n");
	mWorld.draw();
}

bool GameState::Update(const GameTimer& gt)
{
	ProcessInput();

	mWorld.update(gt);

	return true;
}

bool GameState::HandleEvent(WPARAM btnState)
{
	if (d3dUtil::IsKeyDown('P'))
	{
		RequestStackPop();
		RequestStackPush(States::Pause);
	}

	return true;
}

bool GameState::HandleRealTimeInput()
{
	return true;
}

void GameState::ProcessInput()
{
	CommandQueue& commands = mWorld.getCommandQueue();
	mContext->player->HandleEvent(commands);
	mContext->player->HandeRealTimeInput(commands);
}
