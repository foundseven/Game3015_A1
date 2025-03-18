#include "Aircraft.hpp"
#include "Game.hpp"

/**
 * @brief Constructor for Aircraft.
 * @param type The type of aircraft.
 * @param game Pointer to the Game object.
 */
Aircraft::Aircraft(Type type, Game* game) : Entity(game)
	, mType(type)
{
	switch (type)
	{
	case (Eagle):
		mSprite = "Eagle";
		break;
	case (Raptor):
		mSprite = "Raptor";
		break;
	default:
		mSprite = "Eagle";
		break;
	}
}

/**
 * @brief Gets collision category flags for this aircraft
 * @return unsigned int Category flag from Category enumeration
 *
 * @retval Category::PlayerAircraft If aircraft type is Eagle
 * @retval Category::EnemyAircraft For all other types (including Raptor)
 */
unsigned int Aircraft::getCategory() const
{
	switch (mType)
	{
	case Type::Eagle:
		return Category::PlayerAircraft;
	default:
		return Category::EnemyAircraft;
	}
}

/**
 * @brief Executes DirectX 12 rendering commands for the aircraft
 *
 * Configures and submits draw calls using current frame resources:
 * 1. Retrieves constant buffer resources
 * 2. Sets up vertex/index buffers
 * 3. Binds shader resources (textures, materials)
 * 4. Submits indexed draw call
 *
 * @note Skips rendering if mAircraftRitem is null. Requires valid
 *       frame resource setup in Game class.
 */
void Aircraft::drawCurrent() const
{	
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));

	auto objectCB = game->mCurrFrameResource->ObjectCB->Resource();
	auto matCB = game->mCurrFrameResource->MaterialCB->Resource();

	if (mAircraftRitem != nullptr)
	{
		// Vertex buffer setup
		auto vbv = mAircraftRitem->Geo->VertexBufferView();
		auto ibv = mAircraftRitem->Geo->IndexBufferView();

		game->getCmdList()->IASetVertexBuffers(0, 1, &vbv);
		game->getCmdList()->IASetIndexBuffer(&ibv);
		game->getCmdList()->IASetPrimitiveTopology(mAircraftRitem->PrimitiveType);

		// Resource binding
		CD3DX12_GPU_DESCRIPTOR_HANDLE tex(game->mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		tex.Offset(mAircraftRitem->Mat->DiffuseSrvHeapIndex, game->mCbvSrvDescriptorSize);
		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + (UINT64)mAircraftRitem->ObjCBIndex * objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + (UINT64)mAircraftRitem->Mat->MatCBIndex * matCBByteSize;
		game->getCmdList()->SetGraphicsRootDescriptorTable(0, tex);
		game->getCmdList()->SetGraphicsRootConstantBufferView(1, objCBAddress);
		game->getCmdList()->SetGraphicsRootConstantBufferView(3, matCBAddress);
		game->getCmdList()->DrawIndexedInstanced(mAircraftRitem->IndexCount, 1, mAircraftRitem->StartIndexLocation, mAircraftRitem->BaseVertexLocation, 0);
	}
}

/**
 * @brief Constructs render data for the aircraft
 *
 * Creates and configures a RenderItem that contains:
 * - World transformation matrix
 * - Object CBV index
 * - Material reference
 * - Geometry data (currently using placeholder 'boxGeo')
 * - Draw parameters for indexed rendering
 *
 * @note The created RenderItem is added to Game's render item list.
 *       Geometry is currently hardcoded to 'boxGeo' - consider making
 *       this configurable based on aircraft type if using different meshes.
 */
void Aircraft::buildCurrent()
{
	auto render = std::make_unique<RenderItem>();
	renderer = render.get();
	renderer->World = getTransform();
	renderer->ObjCBIndex = game->getRenderItems().size();
	renderer->Mat = game->getMaterials()[mSprite].get();
	renderer->Geo = game->getGeometries()["boxGeo"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderer->IndexCount = renderer->Geo->DrawArgs["box"].IndexCount;
	renderer->StartIndexLocation = renderer->Geo->DrawArgs["box"].StartIndexLocation;
	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["box"].BaseVertexLocation;

	game->getRenderItems().push_back(std::move(render));
}
