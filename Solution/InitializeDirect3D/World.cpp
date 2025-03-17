#define NOMINMAX
#include "World.hpp"

/**
 * @brief Constructor for World.
 * @param game Pointer to the Game object.
 */
World::World(Game* game)
	: mSceneGraph(new SceneNode(game))
	, mGame(game)
	, mPlayerAircraft(nullptr)
	, mBackground(nullptr)
	, mWorldBounds(-3.25f, 3.25f, -1.5f, 2.5f) //Left, Right, Down, Up - this can be changed depending on where you want the player to be
	, mSpawnPosition(0.f, 0.f)
	, mScrollSpeed(1.0f)
{
}

#pragma region Step 14
CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}
#pragma endregion

/**
 * @brief Updates the game world.
 *
 * This method updates the scene graph and handles player aircraft movement within world bounds.
 *
 * @param gt const reference to GameTimer object.
 */
void World::update(const GameTimer& gt)
{
#pragma region Step 15
	mPlayerAircraft->setVelocity(0.0f, 0.0f, 0.0f);
	while (!mCommandQueue.isEmpty())
		mSceneGraph->onCommand(mCommandQueue.pop(), gt);

	PlayerPosition();
	mSceneGraph->update(gt);
	PlayerVelocity();

	//todo: should add some logic in here
	//if (mPlayerAircraft->getWorldPosition().x < mWorldBounds.x
	//	|| mPlayerAircraft->getWorldPosition().x > mWorldBounds.y)
	//{
	//	//keep the player in the bounds of the screen
	//	mPlayerAircraft->setVelocity(XMFLOAT3(mPlayerAircraft->getVelocity().x * -1.0f, 0, 0));
	//}
#pragma endregion

}

/**
 * @brief Draws the game world.
 *
 * This method calls the draw method of the scene graph to render all game objects.
 */
void World::draw()
{
	mSceneGraph->draw();
}

/**
 * @brief Builds the scene graph and initializes game objects.
 *
 * This method creates and positions the player aircraft, enemy aircraft, and background sprite.
 * It also sets up the initial velocities and rotations of these objects.
 */
void World::buildScene()
{
	// Create and set up player aircraft
	std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mGame));
	mPlayerAircraft = player.get();
	mPlayerAircraft->setPosition(0, 0.1, 0.0);
	mPlayerAircraft->setScale(0.5, 0.5, 0.5);
	//enable this
	//to do: set this so the world is moving and not the player?
	//yeah input for assignment 2 will later over rule this i am sure
	mPlayerAircraft->setVelocity(mScrollSpeed, 0.0, 0.0);
	mSceneGraph->attachChild(std::move(player));

	// Create and set up first enemy aircraft
	std::unique_ptr<Aircraft> enemy1(new Aircraft(Aircraft::Raptor, mGame));
	auto raptor = enemy1.get();
	raptor->setPosition(0.5, 0, -1);
	raptor->setScale(1.0, 1.0, 1.0);
	raptor->setWorldRotation(0, 0.0, 0);
	mPlayerAircraft->attachChild(std::move(enemy1));

	// Create and set up second enemy aircraft
	std::unique_ptr<Aircraft> enemy2(new Aircraft(Aircraft::Raptor, mGame));
	auto raptor2 = enemy2.get();
	//raptor2->setPosition(-0.5, 0, 1);
	raptor2->setPosition(-0.5, 0, -1);
	raptor2->setScale(1.0, 1.0, 1.0);
	//raptor2->setWorldRotation(0, XM_PI, 0);
	raptor2->setWorldRotation(0, 0.0, 0);
	mPlayerAircraft->attachChild(std::move(enemy2));

	// Create and set up background sprite
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(mGame));
	mBackground = backgroundSprite.get();
	//mBackground->setPosition(mWorldBounds.left, mWorldBounds.top);
	mBackground->setPosition(0, 0, 0.0);
	mBackground->setScale(10.0, 1.0, 200.0);
	mBackground->setVelocity(0, 0, -mScrollSpeed); //background scrolling enabled
	mSceneGraph->attachChild(std::move(backgroundSprite));

	// Build the scene graph
	mSceneGraph->build();
}

//part of step 15
void World::PlayerPosition()
{
	const float borderDistance = 10.f;

	XMFLOAT3 position = mPlayerAircraft->getWorldPosition();
	position.x = std::max(position.x, mWorldBounds.x);
	position.x = std::min(position.x, mWorldBounds.y);
	position.z = std::max(position.z, mWorldBounds.z);
	position.z = std::min(position.z, mWorldBounds.w);
	mPlayerAircraft->setPosition(position.x, position.y, position.z);
}

void World::PlayerVelocity()
{
	XMFLOAT3 velocity = mPlayerAircraft->getVelocity();

	if (velocity.x != 0.0f && velocity.z != 0.0f)
	{
		mPlayerAircraft->setVelocity(velocity.x / std::sqrt(2.f), velocity.y / std::sqrt(2.f), velocity.z / std::sqrt(2.f));
	}
}
