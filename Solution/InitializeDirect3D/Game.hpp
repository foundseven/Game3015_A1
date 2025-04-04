#include "World.hpp"
#include "Player.hpp"
#include "StateStack.hpp"
#include <dwrite.h>
#include <d2d1.h>


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

	void WaitForGPU();
	//bool CreateText(const wchar_t* text);
	//void DrawTheText();

	//for hills
	float GetHillsHeight(float x, float z)const;
	XMFLOAT3 GetHillsNormal(float x, float z)const;

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();


public: // Changed from private to public

	std::vector<std::unique_ptr<FrameResource>> mFrameResources;

	FrameResource* mCurrFrameResource = nullptr;

	int mCurrFrameResourceIndex = 0;

	int mCurrentMaterialCBIndex = 0;

	int mCurrentDiffuseSrvHeapIndex = 0;

	UINT mCbvSrvDescriptorSize = 0;

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	//step11
	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;

	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;

	//step7
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;

	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	ComPtr<ID3D12PipelineState> mOpaquePSO = nullptr;

	std::vector<std::unique_ptr<RenderItem>> mAllRitems;

	std::vector<RenderItem*> mOpaqueRitems;

	PassConstants mMainPassCB;

	//XMFLOAT3 mEyePos = { 0.0f, 0.0f, -10.0f };
	//XMFLOAT4X4 mView = MathHelper::Identity4x4();
	//XMFLOAT4X4 mProj = MathHelper::Identity4x4();

	//float mTheta = 1.3f * XM_PI;
	//float mPhi = 0.4f * XM_PI;
	//float mRadius = 2.5f;

	POINT mLastMousePos;

	Camera mCamera;

#pragma region Step 19
	
	Player mPlayer;
#pragma endregion

	StateStack mStateStack;

public:

	ID3D12GraphicsCommandList* getCmdList() { return mCommandList.Get(); }


	std::unordered_map<std::string, std::unique_ptr<Material>>& getMaterials() { return mMaterials; }

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>>& getGeometries() { return mGeometries; }

	//text rendering attempt
	private:

		// Direct2D
		ID2D1Factory* pD2DFactory_;
		ID2D1HwndRenderTarget* pRT_;
		ID2D1SolidColorBrush* pBlackBrush_;

		// DirectWrite
		IDWriteFactory* pDWriteFactory_;
		IDWriteTextFormat* pTextFormat_;

		const wchar_t* wszText_;
		UINT32 cTextLength_;

};
