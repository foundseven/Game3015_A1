#pragma once
#include "Entity.hpp"
#include <string>

/**
 * @brief Text rendering entity for UI/menu systems
 *
 * Specialized Entity subclass for displaying dynamic text elements in the game world.
 * Supports menu text with optional flashing behavior for visual emphasis.
 */
class Text : public Entity {

public:
	/**
	 * @brief Enumeration of supported menu text types
	 */
	enum Menu
	{
		MenuText ///< Standard menu text element
	};

public:
	/**
	 * @brief Constructs a Text entity with specific menu configuration
	 * @param menu Menu type identifier from enum
	 * @param state Pointer to parent State object
	 */
	Text(Menu menu, State* state);

private:
    /**
     * @brief Updates text properties each frame
     * @param gt Game timer reference for frame timing
     */
    void updateCurrent(const GameTimer& gt);

    /**
     * @brief Performs state validation checks
     * @param gt Game timer reference for timing-dependent checks
     */
    void check(const GameTimer& gt);

    /**
     * @brief Handles rendering of text entity
     */
    virtual void drawCurrent() const;

    /**
     * @brief Builds text geometry and resources
     */
    virtual void buildCurrent();

private:
    //-------------------------------------------------------------------------
    // Text Configuration
    //-------------------------------------------------------------------------

    Menu mMenu;         ///< Menu type configuration
    std::string mSprite;///< Sprite/texture identifier for text rendering
    bool flash;         ///< Flashing animation enabled state
    bool show;          ///< Current visibility state
};