#include "World.hpp"
#include "Player.hpp"
/**
 * @class Game
 * @brief Main game class managing resources, rendering, and game logic
 *
 * This class inherits from D3DApp and is responsible for initializing the game,
 * managing game resources, handling input, updating game state, and rendering
 * the game world. It serves as the central hub for all game-related operations.
 */
class Game : public D3DApp
{
public:
	/**
	 * @brief Constructor for the Game class.
	 * @param hInstance The HINSTANCE of the application.
	 */
	Game(HINSTANCE hInstance);

	/**
	 * @brief Deleted copy constructor.  Objects of this class cannot be copied.
	 * @param rhs Unused.
	 */
	Game(const Game& rhs) = delete;
	Game& operator=(const Game& rhs) = delete;
	~Game();

	/**
	 * @brief Initializes the game. This includes D3D, Resource creation, etc.
	 * @return true if initialization was successful, false otherwise.
	 */
	virtual bool Initialize()override;

private:
	/**
	 * @brief Handles window resizing.
	 */
	virtual void OnResize() override;

	void ProcessInput();

	/**
	 * @brief Updates the game state.
	 * @param gt A const reference to a GameTimer object.
	 */
	virtual void Update(const GameTimer& gt) override;

	/**
	 * @brief Draws the game scene.
	 * @param gt A const reference to a GameTimer object.
	 */
	virtual void Draw(const GameTimer& gt) override;


	/**
	 * @brief Handles mouse button down events.
	 * @param btnState The state of the mouse buttons.
	 * @param x The x-coordinate of the mouse.
	 * @param y The y-coordinate of the mouse.
	 */
	virtual void OnMouseDown(WPARAM btnState, int x, int y) override;

	/**
	 * @brief Handles mouse button up events.
	 * @param btnState The state of the mouse buttons.
	 * @param x The x-coordinate of the mouse.
	 * @param y The y-coordinate of the mouse.
	 */
	virtual void OnMouseUp(WPARAM btnState, int x, int y) override;

	/**
	 * @brief Handles mouse move events.
	 * @param btnState The state of the mouse buttons.
	 * @param x The x-coordinate of the mouse.
	 * @param y The y-coordinate of the mouse.
	 */
	virtual void OnMouseMove(WPARAM btnState, int x, int y) override;

	/**
	 * @brief Handles keyboard input.
	 * @param gt A const reference to a GameTimer object.
	 */
	//void OnKeyboardInput(const GameTimer& gt);

	/**
	 * @brief Updates the camera.
	 * @param gt A const reference to a GameTimer object.
	 */
	void UpdateCamera(const GameTimer& gt);

	/**
	 * @brief Animates the materials.
	 * @param gt A const reference to a GameTimer object.
	 */
	void AnimateMaterials(const GameTimer& gt);

	/**
	 * @brief Updates the object constant buffers
	 * @param gt A const reference to a GameTimer object
	 *
	 * @note This method may be performance-critical. Consider optimizing
	 *       or parallelizing if it becomes a bottleneck.
	 */
	void UpdateObjectCBs(const GameTimer& gt);

	/**
	 * @brief Updates the material constant buffers.
	 * @param gt A const reference to a GameTimer object.
	 */
	void UpdateMaterialCBs(const GameTimer& gt);

	/**
	 * @brief Updates the main pass constant buffer.
	 * @param gt A const reference to a GameTimer object.
	 */
	void UpdateMainPassCB(const GameTimer& gt);

	/**
	 * @brief Loads the textures.
	 */
	//step5
	void LoadTextures();
	
	/**
	 * @brief Builds the root signature.
	 */
	void BuildRootSignature();

	/**
	 * @brief Builds the descriptor heaps.
	 */
	//step9
	void BuildDescriptorHeaps();

	/**
	 * @brief Builds the shaders and input layout.
	 */
	void BuildShadersAndInputLayout();

	/**
	 * @brief Builds the shape geometry.
	 */
	void BuildShapeGeometry();

	/**
	 * @brief Builds the hill geometry.
	 */
	void BuildHillGeometry();

	/**
	 * @brief Builds the pipeline state objects.
	 */
	void BuildPSOs();

	/**
	 * @brief Builds the frame resources.
	 */
	void BuildFrameResources();

	/**
	 * @brief Builds the materials.
	 */
	void BuildMaterials();

	/**
	 * @brief Builds the render items.
	 */
	void BuildRenderItems();

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
	World mWorld;

#pragma region Step 19
	/**
	* @brief The player object
	*
	* Handles player-specific logic, including input mapping and
	* player state management.
	*/
	Player mPlayer;
#pragma endregion

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
	std::vector<std::unique_ptr<RenderItem>>& getRenderItems() { return mAllRitems; }

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
