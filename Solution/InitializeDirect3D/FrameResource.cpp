#include "FrameResource.h"

/**
 * @brief Constructor for FrameResource.
 * @param device Pointer to the D3D12 device.
 * @param passCount Number of passes.
 * @param objectCount Number of objects.
 * @param materialCount Number of materials.
 */
FrameResource::FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount, UINT materialCount)
{
    ThrowIfFailed(device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CmdListAlloc.GetAddressOf())));

    //FrameCB = std::make_unique<UploadBuffer<FrameConstants>>(device, 1, true);
    PassCB = std::make_unique<UploadBuffer<PassConstants>>(device, passCount, true);
    MaterialCB = std::make_unique<UploadBuffer<MaterialConstants>>(device, materialCount, true);
    ObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(device, objectCount, true);
}

/**
 * @brief Destructor for FrameResource.
 */
FrameResource::~FrameResource()
{

}