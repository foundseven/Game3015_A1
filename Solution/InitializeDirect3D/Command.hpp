#pragma region Step 5
#pragma once
#include "Category.hpp"
#include "../../Common/d3dApp.h"
#include <functional>
#include <cassert>

class SceneNode;

//so creating an input system that creates commands
struct Command
{
	Command();

	std::function<void(SceneNode&, const GameTimer&)> action;
	unsigned int category;
};

template <typename GameObject, typename Function>
std::function<void(SceneNode&, const GameTimer&)> derivedAction(Function fn)
{
    return [=](SceneNode& node, const GameTimer& gameTimer)
        {
            //Check if the cast is safe
            assert(dynamic_cast<GameObject*>(&node) != nullptr);

            //Downcast node and invoke function on it
            fn(static_cast<GameObject&>(node), gameTimer);
        };
}
#pragma endregion