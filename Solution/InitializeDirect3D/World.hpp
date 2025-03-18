#pragma once
#include "SceneNode.hpp"
#include "Aircraft.hpp"
#include "SpriteNode.h"

#pragma region Step 12
#include "CommandQueue.hpp"
#include "Command.hpp"
#pragma endregion

/**
 * @brief Constructs the World object
 * @param game Pointer to the Game object
 *
 * Initializes the scene graph, world bounds, and scroll speed.
 */
class World 
{
public:
	/**
	 * @brief Constructs the World object
	 * @param game Pointer to the Game object
	 *
	 * Initializes the scene graph, world bounds, and scroll speed.
	 */
	explicit							World(Game* window);
	/**
	 * @brief Updates the world.
	 * @param gt const reference to GameTimer object.
	 */
	void								update(const GameTimer& gt);
	/**
	 * @brief Draws the game world.
	 */
	void								draw();

	//void								loadTextures();
	/**
	 * @brief Builds the scene graph and initializes game objects.
	 */
	void								buildScene();


#pragma region Step 13
	/**
	* @brief Retrieves the command queue for the world
	* @return Reference to the CommandQueue
	*/
	CommandQueue&						getCommandQueue();
private:
	CommandQueue						mCommandQueue;

	/**
	 * @brief Adjusts player position to stay within world bounds
	 */
	void PlayerPosition();

	/**
	 * @brief Normalizes player velocity for diagonal movement
	 */
	void PlayerVelocity();
#pragma endregion

private:
	/**
	 * @brief Enumeration of scene layers.
	 */
	enum Layer
	{
		Background,
		Air,
		LayerCount
	};


private:
	Game*								mGame; ///< Pointer to the Game object.

	SceneNode*							mSceneGraph; ///< Root node of the scene graph.
	std::array<SceneNode*, LayerCount>	mSceneLayers; ///< Array of scene layers.

	XMFLOAT4							mWorldBounds; ///< Boundaries of the game world.
	XMFLOAT2		    				mSpawnPosition; ///< Spawn position for new objects.
	float								mScrollSpeed; ///< Scrolling speed of the world.
	Aircraft*							mPlayerAircraft; ///< Pointer to the player's aircraft.
	SpriteNode*							mBackground; ///< Pointer to the background sprite.
	Aircraft*							mEnemy; ///< Pointer to an enemy aircraft.
};
