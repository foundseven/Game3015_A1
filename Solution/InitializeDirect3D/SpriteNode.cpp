#include "SpriteNode.h"
#include "Game.hpp"

/**
 * @brief Constructor for SpriteNode.
 * @param game Pointer to the Game object.
 */
SpriteNode::SpriteNode(Game* game) : Entity(game)
{
}

/**
 * @brief Draws the current sprite node.
 *
 * This method handles the rendering of the sprite node, including setting up
 * the necessary DirectX 12 commands and resources.
 */
void SpriteNode::drawCurrent() const
{
	renderer->World = getTransform();
	renderer->NumFramesDirty++;

	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));
	auto objectCB = game->mCurrFrameResource->ObjectCB->Resource();
	auto matCB = game->mCurrFrameResource->MaterialCB->Resource();
	if (mSpriteNodeRitem != nullptr)
	{
		//logic here
		auto vbv = mSpriteNodeRitem->Geo->VertexBufferView();
		auto ibv = mSpriteNodeRitem->Geo->IndexBufferView();

		game->getCmdList()->IASetVertexBuffers(0, 1, &vbv);
		game->getCmdList()->IASetIndexBuffer(&ibv);
		game->getCmdList()->IASetPrimitiveTopology(mSpriteNodeRitem->PrimitiveType);
		CD3DX12_GPU_DESCRIPTOR_HANDLE tex(game->mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		tex.Offset(mSpriteNodeRitem->Mat->DiffuseSrvHeapIndex, game->mCbvSrvDescriptorSize);
		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + mSpriteNodeRitem->ObjCBIndex * objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + mSpriteNodeRitem->Mat->MatCBIndex * matCBByteSize;
		game->getCmdList()->SetGraphicsRootDescriptorTable(0, tex);
		game->getCmdList()->SetGraphicsRootConstantBufferView(1, objCBAddress);
		game->getCmdList()->SetGraphicsRootConstantBufferView(3, matCBAddress);
		game->getCmdList()->DrawIndexedInstanced(mSpriteNodeRitem->IndexCount, 1, mSpriteNodeRitem->StartIndexLocation, mSpriteNodeRitem->BaseVertexLocation, 0);
	}
}

/**
 * @brief Builds the current sprite node.
 *
 * This method sets up the RenderItem for the sprite node, including its
 * world transform, material, geometry, and other rendering properties.
 */
void SpriteNode::buildCurrent()
{
	auto render = std::make_unique<RenderItem>();
	renderer = render.get();
	renderer->World = getTransform();
	XMStoreFloat4x4(&renderer->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
	renderer->ObjCBIndex = game->getRenderItems().size();
	renderer->Mat = game->getMaterials()["Galaxy"].get();
	renderer->Geo = game->getGeometries()["gridGeo"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderer->IndexCount = renderer->Geo->DrawArgs["grid"].IndexCount;
	renderer->StartIndexLocation = renderer->Geo->DrawArgs["grid"].StartIndexLocation;
	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["grid"].BaseVertexLocation;

	game->getRenderItems().push_back(std::move(render));
}
