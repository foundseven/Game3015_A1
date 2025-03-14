#pragma once
#include "../../Common/d3dApp.h"
#include "../../Common/MathHelper.h"
#include "../../Common/UploadBuffer.h"
#include "../../Common/GeometryGenerator.h"
#include "../../Common/Camera.h"
#include "FrameResource.h"

#pragma region Step 3
#include "Category.hpp"
#pragma endregion

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")

/**
 * @brief Lightweight structure to store parameters for drawing a shape.
 */
struct RenderItem
{
	RenderItem() = default;

	// World matrix of the shape that describes the object's local space
	// relative to the world space, which defines the position, orientation,
	// and scale of the object in the world.
	XMFLOAT4X4 World = MathHelper::Identity4x4(); ///< World matrix of the shape

	XMFLOAT4X4 TexTransform = MathHelper::Identity4x4(); ///< Texture transform matrix

	// Dirty flag indicating the object data has changed and we need to update the constant buffer.
	// Because we have an object cbuffer for each FrameResource, we have to apply the
	// update to each FrameResource.  Thus, when we modify obect data we should set 
	// NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
	int NumFramesDirty = gNumFrameResources; ///< Number of frames the object data has been dirty

	UINT ObjCBIndex = -1; ///< Index into GPU constant buffer

	Material* Mat = nullptr; ///< Material of the object
	MeshGeometry* Geo = nullptr; ///< Geometry of the object

	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; ///< Primitive topology

	// DrawIndexedInstanced parameters.
	UINT IndexCount = 0; ///< Number of indices
	UINT StartIndexLocation = 0; ///< Starting index location
	int BaseVertexLocation = 0; ///< Base vertex location
};

class Game;

#pragma region Step 7
struct Command;
#pragma endregion

/**
 * @brief Represents a node in the scene graph.
 */
class SceneNode
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;

public:
	/**
	 * @brief Constructor for SceneNode.
	 * @param game Pointer to the Game object.
	*/
	SceneNode(Game* game);

	/**
	 * @brief Attaches a child node to this node.
	 * @param child Unique pointer to the child node.
	 */
	void					attachChild(Ptr child);
	/**
	 * @brief Detaches a child node from this node.
	 * @param node Reference to the node to be detached.
	 * @return Unique pointer to the detached node.
	 */
	Ptr						detachChild(const SceneNode& node);
	/**
	 * @brief Updates this node and its children.
	 * @param gt GameTimer object.
	 */
	void					update(const GameTimer& gt);
	/**
	 * @brief Draws this node and its children.
	 */
	void					draw() const;
	/**
	 * @brief Builds this node and its children.
	 */
	void					build();


	/**
	 * @brief Gets the world position of this node.
	 * @return XMFLOAT3 representing the world position.
	 */
	XMFLOAT3				getWorldPosition() const;
	/**
	 * @brief Sets the position of this node.
	 * @param x X-coordinate.
	 * @param y Y-coordinate.
	 * @param z Z-coordinate.
	 */
	void					setPosition(float x, float y, float z);
	/**
	 * @brief Gets the world rotation of this node.
	 * @return XMFLOAT3 representing the world rotation.
	 */
	XMFLOAT3				getWorldRotation() const;
	/**
	 * @brief Sets the rotation of this node.
	 * @param x X-coordinate.
	 * @param y Y-coordinate.
	 * @param z Z-coordinate.
	 */
	void					setWorldRotation(float x, float y, float z);
	/**
	 * @brief Gets the world scale of this node.
	 * @return XMFLOAT3 representing the world scale.
	 */
	XMFLOAT3				getWorldScale() const;
	/**
	 * @brief Sets the scale of this node.
	 * @param x X-coordinate.
	 * @param y Y-coordinate.
	 * @param z Z-coordinate.
	 */
	void					setScale(float x, float y, float z);

	/**
	 * @brief Gets the world transform of this node.
	 * @return XMFLOAT4X4 representing the world transform.
	 */
	XMFLOAT4X4				getWorldTransform() const;
	/**
	 * @brief Gets the local transform of this node.
	 * @return XMFLOAT4X4 representing the local transform.
	 */
	XMFLOAT4X4				getTransform() const;

	/**
	 * @brief Moves this node by the specified amount.
	 * @param x Movement in X-direction.
	 * @param y Movement in Y-direction.
	 * @param z Movement in Z-direction.
	 */
	void					move(float x, float y, float z);

#pragma region Step 8
	//so this will allow us to now act on the command
	void					onCommand(const Command& command, const GameTimer& gt);
	virtual unsigned int    getCategory() const;
#pragma endregion

private:
	/**
	 * @brief Updates this node.
	 * @param gt GameTimer object.
	 */
	virtual void			updateCurrent(const GameTimer& gt);
	/**
	 * @brief Updates all children of this node.
	 * @param gt GameTimer object.
	 */
	void					updateChildren(const GameTimer& gt);

	/**
	 * @brief Draws this node.
	 */
	virtual void			drawCurrent() const;
	/**
	 * @brief Draws all children of this node.
	 */
	void					drawChildren() const;
	/**
	 * @brief Builds this node.
	 */
	virtual void			buildCurrent();
	/**
	 * @brief Builds all children of this node.
	 */
	void					buildChildren();

protected:
	Game*					game; ///< Pointer to the Game object
	RenderItem*				renderer; ///< Pointer to the RenderItem for this node
private:
	XMFLOAT3				mWorldPosition; ///< World position of this node
	XMFLOAT3				mWorldRotation; ///< World rotation of this node
	XMFLOAT3				mWorldScaling; ///< World scaling of this node

	std::vector<Ptr>		mChildren; ///< Vector of child nodes
	SceneNode*				mParent; ///< Pointer to the parent node
};

