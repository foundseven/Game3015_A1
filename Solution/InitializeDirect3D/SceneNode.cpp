#include "SceneNode.hpp"
#include "Game.hpp"

/**
 * @brief Constructor for SceneNode.
 * @param game Pointer to the Game object.
 */
SceneNode::SceneNode(Game* game)
	: mChildren()
	, mParent(nullptr)
	, game(game)
{
	mWorldPosition = XMFLOAT3(0, 0, 0);
	mWorldScaling = XMFLOAT3(1, 1, 1);
	mWorldRotation = XMFLOAT3(0, 0, 0);
}

/**
 * @brief Attaches a child node to this node.
 * @param child Unique pointer to the child node.
 */
void SceneNode::attachChild(Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

/**
 * @brief Detaches a child node from this node.
 * @param node Reference to the node to be detached.
 * @return Unique pointer to the detached node.
 */
SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& p) { return p.get() == &node; });
	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

/**
 * @brief Updates this node and its children.
 * @param gt GameTimer object.
 */
void SceneNode::update(const GameTimer& gt)
{
	updateCurrent(gt);
	updateChildren(gt);
}

/**
 * @brief Updates this node.
 * @param gt GameTimer object.
 */
void SceneNode::updateCurrent(const GameTimer& gt)
{
	// Do nothing by default
}

/**
 * @brief Updates all children of this node.
 * @param gt GameTimer object.
 */
void SceneNode::updateChildren(const GameTimer& gt)
{
	for (Ptr& child : mChildren)
	{
		child->update(gt);
	}
}

/**
 * @brief Draws this node and its children.
 */
void SceneNode::draw() const
{
	drawCurrent();
	drawChildren();
}

/**
 * @brief Draws this node.
 */
void SceneNode::drawCurrent() const
{
	//Empty for now
}

/**
 * @brief Draws all children of this node.
 */
void SceneNode::drawChildren() const
{
	for (const Ptr& child : mChildren)
	{
		child->draw();
	}
}

/**
 * @brief Builds this node and its children.
 */
void SceneNode::build()
{
	buildCurrent();
	buildChildren();
}

/**
 * @brief Builds this node.
 */
void SceneNode::buildCurrent()
{
	//Empty for now
}

/**
 * @brief Builds all children of this node.
 */
void SceneNode::buildChildren()
{
	for (const Ptr& child : mChildren)
	{
		child->build();
	}
}

/**
 * @brief Gets the world position of this node.
 * @return XMFLOAT3 representing the world position.
 */
XMFLOAT3 SceneNode::getWorldPosition() const
{
	return mWorldPosition;
}

/**
 * @brief Sets the position of this node.
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @param z Z-coordinate.
 */
void SceneNode::setPosition(float x, float y, float z)
{
	mWorldPosition = XMFLOAT3(x, y, z);
}

/**
 * @brief Gets the world rotation of this node.
 * @return XMFLOAT3 representing the world rotation.
 */
XMFLOAT3 SceneNode::getWorldRotation() const
{
	return mWorldRotation;
}

/**
 * @brief Sets the world rotation of this node.
 * @param x Rotation around X-axis.
 * @param y Rotation around Y-axis.
 * @param z Rotation around Z-axis.
 */
void SceneNode::setWorldRotation(float x, float y, float z)
{
	mWorldRotation = XMFLOAT3(x, y, z);
}

/**
 * @brief Gets the world scale of this node.
 * @return XMFLOAT3 representing the world scale.
 */
XMFLOAT3 SceneNode::getWorldScale() const
{
	return mWorldScaling;
}

/**
 * @brief Sets the scale of this node.
 * @param x Scale in X-direction.
 * @param y Scale in Y-direction.
 * @param z Scale in Z-direction.
 */
void SceneNode::setScale(float x, float y, float z)
{
	mWorldScaling = XMFLOAT3(x, y, z);
}

/**
 * @brief Gets the world transform of this node.
 * @return XMFLOAT4X4 representing the world transform.
 */
XMFLOAT4X4 SceneNode::getWorldTransform() const
{
	XMFLOAT4X4 transform = MathHelper::Identity4x4();
	XMMATRIX T = XMLoadFloat4x4(&transform);

	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
	{
		auto nodeTransform = node->getTransform();
		XMMATRIX Tp = XMLoadFloat4x4(&nodeTransform);
		T = Tp * T;
	}
	XMStoreFloat4x4(&transform, T);

	return transform;
}

/**
 * @brief Gets the local transform of this node.
 * @return XMFLOAT4X4 representing the local transform.
 */
XMFLOAT4X4 SceneNode::getTransform() const
{
	XMFLOAT4X4 transform;
	XMStoreFloat4x4(&transform, XMMatrixScaling(mWorldScaling.x, mWorldScaling.y, mWorldScaling.z) *
		XMMatrixRotationX(mWorldRotation.x) *
		XMMatrixRotationY(mWorldRotation.y) *
		XMMatrixRotationZ(mWorldRotation.z) *
		XMMatrixTranslation(mWorldPosition.x, mWorldPosition.y, mWorldPosition.z));
	return transform;
}

/**
 * @brief Moves this node by the specified amount.
 * @param x Movement in X-direction.
 * @param y Movement in Y-direction.
 * @param z Movement in Z-direction.
 */
void SceneNode::move(float x, float y, float z)
{
	mWorldPosition.x += x;
	mWorldPosition.y += y;
	mWorldPosition.z += z;
}

#pragma region Step 9
void SceneNode::onCommand(const Command& command, const GameTimer& gt)
{
	//Command on current node, if category matches
	if (command.category & getCategory())
		command.action(*this, gt);

	//cmd children
	for (Ptr& child : mChildren)
		child->onCommand(command, gt);

}

unsigned int SceneNode::getCategory() const
{
	return Category::Scene;
}

#pragma endregion
