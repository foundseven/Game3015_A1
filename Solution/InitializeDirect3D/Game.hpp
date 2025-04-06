#include "World.hpp"
#include "Player.hpp"
#include "StateStack.hpp"
#include <dwrite.h>
#include <d2d1.h>

/**
 * @brief Core game application class inheriting from Direct3D base
 *
 * Manages:
 * - Direct3D/2D rendering systems
 * - Game state stack
 * - Resource management (materials, geometries, textures)
 * - Camera and player systems
 * - Frame resource management
 */
class Game : public D3DApp
{
public:
	/**
	 * @brief Constructs the game application
	 * @param hInstance Application instance handle
	 */
	Game(HINSTANCE hInstance);

	// Delete copy constructor/assignment
	Game(const Game& rhs) = delete;
	Game& operator=(const Game& rhs) = delete;

	/**
	 * @brief Destructor for resource cleanup
	 */
	~Game();

	/**
	 * @brief Initializes game systems
	 * @return True if successful, false otherwise
	 * @override D3DApp override
	 */
	virtual bool Initialize() override;

public:
	//-------------------------------------------------------------------------
	// Core Loop Overrides
	//-------------------------------------------------------------------------

	/**
	 * @brief Handles window resize events
	 * @override D3DApp override
	 */
	virtual void OnResize() override;

	/**
	 * @brief Main update loop
	 * @param gt Game timer with frame timing
	 * @override D3DApp override
	 */
	virtual void Update(const GameTimer& gt) override;

	/**
	 * @brief Main rendering loop
	 * @param gt Game timer with frame timing
	 * @override D3DApp override
	 */
	virtual void Draw(const GameTimer& gt) override;

	//-------------------------------------------------------------------------
	// Input Handling
	//-------------------------------------------------------------------------

	virtual void OnMouseDown(WPARAM btnState, int x, int y) override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y) override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y) override;
	virtual void OnKeyDown(WPARAM btnState) override;

	//-------------------------------------------------------------------------
	// Initialization Helpers
	//-------------------------------------------------------------------------

	/**
	 * @brief Registers game states with the state stack
	 */
	void RegisterStates();

	/**
	 * @brief Creates a texture resource
	 * @param Name Unique identifier for the texture
	 * @param FileName Path to texture file
	 */
	void CreateTexture(std::string Name, std::wstring FileName);

	/**
	 * @brief Creates a material definition
	 * @param Name Unique identifier for the material
	 * @param DiffuseAlbedo Base color properties
	 * @param FresnelR0 Reflectance properties
	 * @param Roughness Surface roughness value
	 */
	void CreateMaterials(std::string Name, XMFLOAT4 DiffuseAlbedo, XMFLOAT3 FresnelR0, float Roughness);

	//-------------------------------------------------------------------------
	// Rendering System
	//-------------------------------------------------------------------------

	void LoadTextures();  ///< Loads all texture resources
	void BuildRootSignature();  ///< Creates root signature
	void BuildDescriptorHeaps();  ///< Builds descriptor heaps
	void BuildShadersAndInputLayout();  ///< Compiles shaders and defines input layout
	void BuildShapeGeometry();  ///< Creates primitive geometries
	void BuildHillGeometry();  ///< Creates terrain geometry
	void BuildPSOs();  ///< Creates pipeline state objects
	void BuildFrameResources(int renderItemCount);  ///< Creates frame resources
	void BuildMaterials();  ///< Initializes default materials
	void ResetFrameResources();  ///< Resets frame resource allocation

	//-------------------------------------------------------------------------
	// Camera & View
	//-------------------------------------------------------------------------

	void UpdateCamera(const GameTimer& gt);  ///< Updates camera transforms
	float GetHillsHeight(float x, float z) const;  ///< Gets terrain height at position
	XMFLOAT3 GetHillsNormal(float x, float z) const;  ///< Gets terrain normal at position

	//-------------------------------------------------------------------------
	// Constant Buffer Updates
	//-------------------------------------------------------------------------

	void UpdateObjectCBs(const GameTimer& gt);  ///< Updates object constant buffers
	void UpdateMaterialCBs(const GameTimer& gt);  ///< Updates material constant buffers
	void UpdateMainPassCB(const GameTimer& gt);  ///< Updates pass constants
	void AnimateMaterials(const GameTimer& gt);  ///< Handles material animations

	//-------------------------------------------------------------------------
	// Resource Access
	//-------------------------------------------------------------------------

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();  ///< Gets default static samplers
	void WaitForGPU();  ///< Synchronizes CPU/GPU execution

	//-------------------------------------------------------------------------
	// Public Members (Consider Refactoring)
	//-------------------------------------------------------------------------
	
	std::vector<std::unique_ptr<FrameResource>> mFrameResources; ///< Frame resource ring buffer
	FrameResource* mCurrFrameResource = nullptr; ///< Current frame resource
	int mCurrFrameResourceIndex = 0; ///< Current frame resource index

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries; ///< Geometry resources
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials; ///< Material definitions
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures; ///< Texture resources

	int mCurrentMaterialCBIndex = 0;

	int mCurrentDiffuseSrvHeapIndex = 0;

	UINT mCbvSrvDescriptorSize = 0;

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;

	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	ComPtr<ID3D12PipelineState> mOpaquePSO = nullptr;

	std::vector<std::unique_ptr<RenderItem>> mAllRitems;

	std::vector<RenderItem*> mOpaqueRitems;

	PassConstants mMainPassCB;

	POINT mLastMousePos;

	Camera mCamera;

	Player mPlayer;  ///< Player entity
	StateStack mStateStack;  ///< Game state manager

	//-------------------------------------------------------------------------
	// Accessors
	//-------------------------------------------------------------------------

	ID3D12GraphicsCommandList* getCmdList() { return mCommandList.Get(); }
	std::unordered_map<std::string, std::unique_ptr<Material>>& getMaterials() { return mMaterials; }
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>>& getGeometries() { return mGeometries; }

private:
	//-------------------------------------------------------------------------
	// Direct2D/DirectWrite Members
	//-------------------------------------------------------------------------

	ID2D1Factory* pD2DFactory_ = nullptr;  ///< Direct2D factory
	ID2D1HwndRenderTarget* pRT_ = nullptr;  ///< Direct2D render target
	ID2D1SolidColorBrush* pBlackBrush_ = nullptr;  ///< Default text brush

	IDWriteFactory* pDWriteFactory_ = nullptr;  ///< DirectWrite factory
	IDWriteTextFormat* pTextFormat_ = nullptr;  ///< Text formatting

	const wchar_t* wszText_ = nullptr;  ///< Text buffer
	UINT32 cTextLength_ = 0;  ///< Text buffer length
};
