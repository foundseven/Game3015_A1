#include "World.hpp"
#include "Player.hpp"
#include "StateStack.hpp"

class Game : public D3DApp
{
public:
	
	Game(HINSTANCE hInstance);


	Game(const Game& rhs) = delete;
	Game& operator=(const Game& rhs) = delete;
	~Game();

	virtual bool Initialize()override;


private:
	
	virtual void OnResize() override;

	//void ProcessInput();
	void RegisterStates();

	virtual void Update(const GameTimer& gt) override;
	virtual void Draw(const GameTimer& gt) override;

	virtual void OnMouseDown(WPARAM btnState, int x, int y) override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y) override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y) override;

	virtual void OnKeyDown(WPARAM btnState)override;

	void UpdateCamera(const GameTimer& gt);
	void AnimateMaterials(const GameTimer& gt);
	void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMaterialCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);

	void CreateTexture(std::string Name, std::wstring FileName);
	void CreateMaterials(std::string Name, XMFLOAT4 DiffuseAlbedo, XMFLOAT3 FresnelR0, float Roughness);

public:
	/**
	 * @brief Loads the textures.
	 */
	//step5
	void LoadTextures();
	void BuildRootSignature();
	void BuildDescriptorHeaps();
	void BuildShadersAndInputLayout();
	void BuildShapeGeometry();
	void BuildHillGeometry();
	void BuildPSOs();
	void BuildFrameResources(int renderItemCount);
	void BuildMaterials();
	//void BuildRenderItems();

	void ResetFrameResources();

	/**
	 * @brief Draws the render items
	 * @param cmdList The command list to draw with
	 * @param ritems The render items to draw
	 *
	 * @see RenderItem
	 * @see ID3D12GraphicsCommandList
	 */
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);

	//for hills
	float GetHillsHeight(float x, float z)const;
	XMFLOAT3 GetHillsNormal(float x, float z)const;

	/**
	 * @brief Returns an array of static sampler descriptions.
	 * @return An array of const CD3DX12_STATIC_SAMPLER_DESC objects.
	 */
	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();


public: // Changed from private to public
	/**
	 * @brief A vector of unique pointers to FrameResource objects.
	 */
	std::vector<std::unique_ptr<FrameResource>> mFrameResources;

	/**
	 * @brief A raw pointer to the current FrameResource.
	 */
	FrameResource* mCurrFrameResource = nullptr;

	/**
	 * @brief The index of the current FrameResource.
	 */
	int mCurrFrameResourceIndex = 0;

	int mCurrentMaterialCBIndex = 0;

	int mCurrentDiffuseSrvHeapIndex = 0;

	/**
	 * @brief The size of a CBV/SRV descriptor on the GPU.
	 */
	UINT mCbvSrvDescriptorSize = 0;

	/**
	 * @brief The root signature used in the game.
	 */
	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	/**
	 * @brief The descriptor heap for SRVs.
	 */
	//step11
	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;

	/**
	 * @brief A map of mesh geometries, indexed by name.
	 */
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;

	/**
	 * @brief A map of materials, indexed by name.
	 */
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;

	/**
	 * @brief A map of textures, indexed by name.
	 */
	//step7
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;

	/**
	 * @brief A map of shaders, indexed by name.
	 */
	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;

	/**
	 * @brief The input layout for the vertex shader.
	 */
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	/**
	 * @brief The pipeline state object for opaque rendering.
	 */
	ComPtr<ID3D12PipelineState> mOpaquePSO = nullptr;

	/**
	 * @brief A list of all render items in the scene.
	 */
	std::vector<std::unique_ptr<RenderItem>> mAllRitems;

	/**
	 * @brief A vector of render items to be rendered with the opaque PSO.
	 */
	std::vector<RenderItem*> mOpaqueRitems;

	/**
	 * @brief The constant buffer for pass-specific data.
	 */
	PassConstants mMainPassCB;

	//XMFLOAT3 mEyePos = { 0.0f, 0.0f, -10.0f };
	//XMFLOAT4X4 mView = MathHelper::Identity4x4();
	//XMFLOAT4X4 mProj = MathHelper::Identity4x4();

	//float mTheta = 1.3f * XM_PI;
	//float mPhi = 0.4f * XM_PI;
	//float mRadius = 2.5f;

	POINT mLastMousePos;

	/**
	 * @brief The camera used for rendering the game world
	 *
	 * This camera object manages the view and projection matrices,
	 * as well as camera movement and orientation.
	 */
	Camera mCamera;

	/**
	 * @brief The game world object
	 *
	 * Manages all entities, scene graph, and game-specific logic
	 * that isn't directly related to rendering or input handling.
	 */
	//World mWorld;

#pragma region Step 19
	/**
	* @brief The player object
	*
	* Handles player-specific logic, including input mapping and
	* player state management.
	*/
	Player mPlayer;
#pragma endregion

	StateStack mStateStack;

public:
	/**
	 * @brief Returns the command list.
	 * @return A pointer to the ID3D12GraphicsCommandList.
	 */
	ID3D12GraphicsCommandList* getCmdList() { return mCommandList.Get(); }

	/**
	 * @brief Returns the list of render items.
	 * @return A reference to the vector of unique pointers to RenderItem objects.
	 */
	//std::vector<std::unique_ptr<RenderItem>>& getRenderItems() { return mAllRitems; }

	/**
	 * @brief Returns the list of materials.
	 * @return A reference to the map of unique pointers to Material objects.
	 */
	std::unordered_map<std::string, std::unique_ptr<Material>>& getMaterials() { return mMaterials; }

	/**
	 * @brief Returns the list of geometries.
	 * @return A reference to the unordered map of mesh geometries.
	 */
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>>& getGeometries() { return mGeometries; }
};
