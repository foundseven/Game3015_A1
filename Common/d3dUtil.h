#pragma once

#include <windows.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>
#include "d3dx12.h"
#include "DDSTextureLoader.h"
#include "MathHelper.h"

extern const int gNumFrameResources;

/**
 * @brief Sets a debug name for a Direct3D object
 * @param obj Pointer to the DXGI object
 * @param name Null-terminated string containing the debug name
 */
inline void d3dSetDebugName(IDXGIObject* obj, const char* name)
{
	if (obj)
	{
		obj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(name), name);
	}
}

/**
 * @brief Sets a debug name for a Direct3D device
 * @param obj Pointer to the ID3D12Device object
 * @param name Null-terminated string containing the debug name
 */
inline void d3dSetDebugName(ID3D12Device* obj, const char* name)
{
	if (obj)
	{
		obj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(name), name);
	}
}

/**
 * @brief Sets a debug name for a Direct3D device child object
 * @param obj Pointer to the ID3D12DeviceChild object
 * @param name Null-terminated string containing the debug name
 */
inline void d3dSetDebugName(ID3D12DeviceChild* obj, const char* name)
{
	if (obj)
	{
		obj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(name), name);
	}
}

/**
 * @brief Converts an ANSI string to a wide string
 * @param str ANSI string to convert
 * @return Wide string representation of the input
 */
inline std::wstring AnsiToWString(const std::string& str)
{
	WCHAR buffer[512];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
	return std::wstring(buffer);
}

class d3dUtil
{
public:
	/**
	 * @brief Checks if a key is currently pressed
	 * @param vkeyCode Virtual key code to check
	 * @return True if key is pressed, false otherwise
	 */
	static bool IsKeyDown(int vkeyCode);

	static std::string ToString(HRESULT hr);

	/**
	 * @brief Calculates the required byte size for a constant buffer
	 * @param byteSize Requested byte size
	 * @return Aligned byte size (multiple of 256)
	 */
	static UINT CalcConstantBufferByteSize(UINT byteSize)
	{
		// Constant buffers must be a multiple of the minimum hardware
		// allocation size (usually 256 bytes).  So round up to nearest
		// multiple of 256.  We do this by adding 255 and then masking off
		// the lower 2 bytes which store all bits < 256.
		// Example: Suppose byteSize = 300.
		// (300 + 255) & ~255
		// 555 & ~255
		// 0x022B & ~0x00ff
		// 0x022B & 0xff00
		// 0x0200
		// 512
		return (byteSize + 255) & ~255;
	}

	/**
	 * @brief Loads a binary file into a blob
	 * @param filename Filename to load
	 * @return ComPtr to the loaded blob
	 */
	static Microsoft::WRL::ComPtr<ID3DBlob> LoadBinary(const std::wstring& filename);

	/**
	 * @brief Creates a default buffer on the GPU and uploads data to it
	 * @param device Direct3D 12 device
	 * @param cmdList Command list to execute the upload
	 * @param initData Pointer to the initial data
	 * @param byteSize Size of the data in bytes
	 * @param[out] uploadBuffer Upload buffer that must persist until command list execution
	 * @return ComPtr to the created default buffer
	 */
	static Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* cmdList,
		const void* initData,
		UINT64 byteSize,
		Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);

	/**
	 * @brief Compiles a shader from file
	 * @param filename Filename of the shader
	 * @param defines Shader macros
	 * @param entrypoint Entry point of the shader
	 * @param target Shader target (e.g., vs_5_0)
	 * @return ComPtr to the compiled shader blob
	 */
	static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
		const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target);
};

/**
 * @brief Exception class for Direct3D errors
 */
class DxException
{
public:
	DxException() = default;
	/**
	 * @brief Constructor for the exception
	 * @param hr HRESULT error code
	 * @param functionName Name of the function that failed
	 * @param filename Filename where the error occurred
	 * @param lineNumber Line number where the error occurred
	 */
	DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber);

	/**
	 * @brief Converts the exception to a string
	 * @return Wide string representation of the exception
	 */
	std::wstring ToString()const;

	HRESULT ErrorCode = S_OK;             ///< HRESULT error code
	std::wstring FunctionName;            ///< Name of the function that failed
	std::wstring Filename;                ///< Filename where the error occurred
	int LineNumber = -1;                  ///< Line number where the error occurred
};

/**
 * @brief Structure defining a subrange of geometry in a mesh
 */
struct SubmeshGeometry
{
	UINT IndexCount = 0;           ///< Number of indices
	UINT StartIndexLocation = 0;   ///< Start index location
	INT BaseVertexLocation = 0;    ///< Base vertex location

	// Bounding box of the geometry defined by this submesh. 
	// This is used in later chapters of the book.
	DirectX::BoundingBox Bounds;  ///< Bounding box
};

/**
 * @brief Structure defining a mesh geometry
 */
struct MeshGeometry
{
	// Give it a name so we can look it up by name.
	std::string Name;                         ///< Name of the mesh

	// System memory copies.  Use Blobs because the vertex/index format can be generic.
	// It is up to the client to cast appropriately. 

	Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU = nullptr;  ///< Vertex buffer in CPU memory
	Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU = nullptr;   ///< Index buffer in CPU memory
	Microsoft::WRL::ComPtr<ID3DBlob> ColorBufferCPU = nullptr;   ///< Color buffer in CPU memory


	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;  ///< Vertex buffer in GPU memory
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;   ///< Index buffer in GPU memory
	Microsoft::WRL::ComPtr<ID3D12Resource> ColorBufferGPU = nullptr;   ///< Color buffer in GPU memory


	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;  ///< Vertex buffer upload heap
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;   ///< Index buffer upload heap
	Microsoft::WRL::ComPtr<ID3D12Resource> ColorBufferUploader = nullptr;   ///< Color buffer upload heap
	UINT VertexByteStride = 0;               ///< Vertex byte stride
	UINT VertexBufferByteSize = 0;           ///< Vertex buffer byte size
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;  ///< Index format
	UINT IndexBufferByteSize = 0;           ///< Index buffer byte size
	UINT ColorByteStride = 0;               ///< Color byte stride
	UINT ColorBufferByteSize = 0;           ///< Color buffer byte size

	// A MeshGeometry may store multiple geometries in one vertex/index buffer.
	// Use this container to define the Submesh geometries so we can draw
	// the Submeshes individually.

	std::unordered_map<std::string, SubmeshGeometry> DrawArgs;  ///< Draw arguments for submeshes

	/**
	 * @brief Returns the vertex buffer view
	 * @return D3D12_VERTEX_BUFFER_VIEW
	 */
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		vbv.StrideInBytes = VertexByteStride;
		vbv.SizeInBytes = VertexBufferByteSize;

		return vbv;
	}

	/**
	 * @brief Returns the index buffer view
	 * @return D3D12_INDEX_BUFFER_VIEW
	 */
	D3D12_INDEX_BUFFER_VIEW IndexBufferView()const
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
		ibv.Format = IndexFormat;
		ibv.SizeInBytes = IndexBufferByteSize;

		return ibv;
	}

	/**
	 * @brief Returns the color buffer view
	 * @return D3D12_VERTEX_BUFFER_VIEW
	 */
	D3D12_VERTEX_BUFFER_VIEW ColorBufferView()const
	{
		D3D12_VERTEX_BUFFER_VIEW cbv;
		cbv.BufferLocation = ColorBufferGPU->GetGPUVirtualAddress();
		cbv.StrideInBytes = ColorByteStride;
		cbv.SizeInBytes = ColorBufferByteSize;

		return cbv;
	}

	/**
	 * @brief Disposes of the uploaders
	 */
	 // We can free this memory after we finish upload to the GPU.
	void DisposeUploaders()
	{
		VertexBufferUploader = nullptr;
		IndexBufferUploader = nullptr;
		ColorBufferUploader = nullptr;
	}
};

/**
 * @brief Structure defining a light
 */
struct Light
{
	DirectX::XMFLOAT3 Strength = { 0.5f, 0.5f, 0.5f };     ///< Light strength
	float FalloffStart = 1.0f;                              ///< Point/spot light only: Falloff start distance
	DirectX::XMFLOAT3 Direction = { 0.0f, -1.0f, 0.0f };    ///< Directional/spot light only: Light direction
	float FalloffEnd = 10.0f;                               ///< Point/spot light only: Falloff end distance
	DirectX::XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };       ///< Point/spot light only: Light position
	float SpotPower = 64.0f;                                ///< Spot light only: Spot power
};

#define MaxLights 16	///< Maximum number of lights

/**
 * @brief Structure defining material constants
 */
struct MaterialConstants
{
	DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };  ///< Diffuse albedo
	DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };         ///< Fresnel reflectance at normal incidence
	float Roughness = 0.25f;                                        ///< Surface roughness

	// Used in texture mapping.
	DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();    ///< Material transform
};

/**
 * @brief Structure defining a material
 */
struct Material
{
	// Unique material name for lookup.
	std::string Name;                    ///< Name of the material

	// Index into constant buffer corresponding to this material.
	int MatCBIndex = -1;                ///< Material constant buffer index

	// Index into SRV heap for diffuse texture.
	int DiffuseSrvHeapIndex = -1;        ///< Diffuse texture SRV index

	// Index into SRV heap for normal texture.
	int NormalSrvHeapIndex = -1;         ///< Normal texture SRV index

	// Dirty flag indicating the material has changed and we need to update the constant buffer.
	// Because we have a material constant buffer for each FrameResource, we have to apply the
	// update to each FrameResource.  Thus, when we modify a material we should set 
	// NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
	int NumFramesDirty = gNumFrameResources;   ///< Number of frames dirty

	// Material constant buffer data used for shading.
	DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };  ///< Diffuse albedo
	DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };         ///< Fresnel reflectance at normal incidence
	float Roughness = .25f;                                        ///< Surface roughness
	DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();    ///< Material transform
};

/**
 * @brief Structure defining a texture
 */
struct Texture
{
	// Unique material name for lookup.
	std::string Name;                                 ///< Name of the texture

	std::wstring Filename;                            ///< Filename of the texture

	Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;  ///< Texture resource
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadHeap = nullptr;  ///< Texture upload heap
};

/**
 * @brief Macro to throw a DxException if a HRESULT failed
 */
#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                                              \
{                                                                     \
    HRESULT hr__ = (x);                                               \
    std::wstring wfn = AnsiToWString(__FILE__);                       \
    if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
}
#endif

 /**
  * @brief Macro to release a COM object
  */
#ifndef ReleaseCom
#define ReleaseCom(x) { if(x){ x->Release(); x = 0; } }
#endif