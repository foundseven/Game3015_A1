#pragma once
#include "Entity.hpp"
#include <string>

class Text : public Entity {

public:
	enum Menu
	{
		MenuText
	};

public:
	Text(Menu menu, State* state);

private:
	void updateCurrent(const GameTimer& gt);
	void check(const GameTimer& gt);
	virtual void drawCurrent() const;
	virtual void buildCurrent();

private:
	Menu mMenu;
	std::string	mSprite;
	bool flash;
	bool show;
};