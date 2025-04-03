#pragma once
#include "Entity.hpp"

/**
 * @brief Represents a sprite node in the game, inheriting from Entity.
 */
class SpriteNode :
    public Entity
{
public:
	/**
	 * @brief Constructor for SpriteNode.
	 * @param game Pointer to the Game object.
	 */
	SpriteNode(State* state);

	/**
	 * @brief Pointer to the RenderItem for this sprite node.
	 */
	RenderItem* mSpriteNodeRitem;
	void SetDrawName(std::string Mat, std::string Geo, std::string DrawName);

private:
	/**
	 * @brief Draws the current sprite node.
	 */
	virtual void		drawCurrent() const;
	/**
	 * @brief Builds the current sprite node.
	 */
	virtual void		buildCurrent();

	std::string mMat;
	std::string mGeo;
	std::string mDrawName;
};
