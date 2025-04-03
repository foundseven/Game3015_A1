#pragma once
#include "StateStack.hpp"
#include "../../Common/d3dApp.h"
#include "FrameResource.h"
#include "SceneNode.hpp"

#include <memory>

namespace sf
{
	class RenderWindow;
}

namespace States
{
	enum ID
	{
		None,
		Title,
		Menu,
		Game,
		Loading,
		Pause
	};
}

class StateStack;
class Player;
class Game;
class SceneNode;

class State
{
public:
	typedef std::unique_ptr<State> StatePtr;

	struct Context
	{
		Context(Game* _game, Player* _player);

		//game holds the textures and what not
		Game*					game;
		Player*					player;
	};
public:
	State(StateStack* stack, Context* context);
	virtual						~State();
	virtual void				Draw() = 0;

	virtual bool				Update(const GameTimer& gt) = 0;
	virtual bool				HandleEvent(WPARAM btnState) = 0;
	virtual bool				HandleRealTimeInput() = 0;

protected:
	void						RequestStackPush(States::ID stateID);
	void						RequestStackPop();
	void						RequestStateClear();

	Context*					GetContext() const;

private:
	StateStack*					mStack;
	Context*					mContext;
};

