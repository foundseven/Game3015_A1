#pragma once
#include "SceneNode.hpp"
#include "Aircraft.hpp"
#include "SpriteNode.h"

/**
 * @brief Represents the game world, managing all game objects and their interactions.
 */
class World 
{
public:
	/**
	 * @brief Constructor for World.
	 * @param window Pointer to the Game object.
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
