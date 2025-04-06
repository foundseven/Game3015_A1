#include "GameState.hpp"
#include "Game.hpp"
#include <windows.h>

/**
 * @brief Constructs the core gameplay state
 * @param stack State stack manager
 * @param context Shared game context
 *
 * Initializes:
 * - Game world simulation
 * - Pause state UI elements
 * - Rendering resources
 * - Input command system
 */
GameState::GameState(StateStack* stack, Context* context)
	: State(stack, context)
	, mWorld(this)
	, mPauseStateSceneGraph(std::make_unique<SceneNode>(this))
{
	// Reset rendering resources
	mAllRitems.clear();
	mContext->game->ResetFrameResources();
	mContext->game->BuildMaterials();

	// Initialize game world
	mWorld.buildScene();

	//-------------------------------------------------------------------------
	// Pause State UI Setup
	//-------------------------------------------------------------------------
	std::unique_ptr<SpriteNode> PauseSprite = std::make_unique<SpriteNode>(this);
	PauseSprite->SetDrawName("PauseText", "boxGeo", "box");
	PauseSprite->setScale(3, 1, 3);
	PauseSprite->setPosition(0, 1, 0);
	mPauseStateSceneGraph->attachChild(std::move(PauseSprite));
	
	mPauseStateSceneGraph->build();

	// Finalize resource setup
	mContext->game->BuildFrameResources(mAllRitems.size());
}

/**
 * @brief Destructor for resource cleanup
 */
GameState::~GameState()
{
}

/**
 * @brief Renders game world and UI elements
 */
void GameState::Draw()
{
	OutputDebugStringA("Drawing game frame...\n");
	mWorld.draw();
}

/**
 * @brief Updates game world simulation
 * @param gt Game timer with frame timing
 * @return True to continue updates in lower states
 */
bool GameState::Update(const GameTimer& gt)
{
	mWorld.update(gt);
	return true;
}

/**
 * @brief Handles discrete input events
 * @param btnState Bitmask of button states
 * @return True to consume event, false to propagate
 */
bool GameState::HandleEvent(WPARAM btnState)
{
	if (d3dUtil::IsKeyDown('P'))
	{
		RequestStackPush(States::Pause);
	}

	return true;
}

/**
 * @brief Handles real-time input processing
 * @return True to consume input, false to propagate
 */
bool GameState::HandleRealTimeInput()
{
	ProcessInput();

	return true;
}

/**
 * @brief Processes player input commands
 *
 * Routes input through:
 * - Event-based commands
 * - Continuous control commands
 */
void GameState::ProcessInput()
{
	CommandQueue& commands = mWorld.getCommandQueue();
	mContext->player->HandleEvent(commands);
	mContext->player->HandeRealTimeInput(commands);
}
