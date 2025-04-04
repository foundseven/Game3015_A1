#include "Game.hpp"
#include "StateStack.hpp"
#include "TitleState.hpp"
#include "GameState.hpp"


const int gNumFrameResources = 3;

/**
 * @brief Constructor for the Game class.
 * @param hInstance The HINSTANCE of the application.
 */
Game::Game(HINSTANCE hInstance)
	: D3DApp(hInstance)
	//, mWorld(this) // Pass 'this' pointer to the World constructor
	, mPlayer()
	, mStateStack(State::Context(this, &mPlayer))
{
}

/**
 * @brief Destructor for the Game class.
 *
 * Flushes the command queue to ensure all GPU commands are completed
 * before releasing resources.
 */
Game::~Game()
{
	if (md3dDevice != nullptr)
		FlushCommandQueue();
}

/**
 * @brief Initializes the game.
 *
 * This method initializes D3D, creates resources, loads textures,
 * builds the root signature, descriptor heaps, shaders, geometry,
 * materials, render items, frame resources, and pipeline state objects.
 *
 * @return true if initialization was successful, false otherwise.
 */
bool Game::Initialize()
{
	if (!D3DApp::Initialize())
		return false;

	// Set initial camera position and orientation
	mCamera.SetPosition(0, 8, 0);
	mCamera.Pitch(3.14 / 2);

	// Reset the command list to prep for initialization commands.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	// Get the increment size of a descriptor in this heap type
	mCbvSrvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Build the core game resources
	LoadTextures();
	BuildRootSignature();
	BuildDescriptorHeaps();
	BuildShadersAndInputLayout();
	BuildShapeGeometry();
	BuildHillGeometry();
	BuildMaterials();
	RegisterStates();
	mStateStack.pushState(States::Title);
	//CreateText();

	BuildPSOs();

	// Execute the initialization commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	FlushCommandQueue();

	return true;
}

/**
 * @brief Handles window resizing.
 *
 * Updates the aspect ratio and recomputes the projection matrix
 * based on the new window dimensions.
 */
void Game::OnResize()
{
	D3DApp::OnResize();

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	//XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	//XMStoreFloat4x4(&mProj, P);

	// Update the camera projection matrix
	mCamera.SetLens(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
}

/**
 * @brief Updates the game state.
 *
 * This method processes input, updates the game world, updates the camera,
 * cycles through frame resources, waits for the GPU to complete previous
 * commands, animates materials, and updates constant buffers.
 *
 * @param gt A const reference to a GameTimer object.
 */
void Game::Update(const GameTimer& gt)
{
	// Handle input and game world updates
	//ProcessInput();
	//mWorld.update(gt);
	mStateStack.Update(gt);
	mStateStack.handleRealTimeInput();

	if (mStateStack.isEmpty())
	{
		PostQuitMessage(0);
		return;
	}

	UpdateCamera(gt);

	// Cycle through the circular frame resource array.
	mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % gNumFrameResources;
	mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();

	// Wait for GPU completion
	if (mCurrFrameResource->Fence != 0 && mFence->GetCompletedValue() < mCurrFrameResource->Fence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(mFence->SetEventOnCompletion(mCurrFrameResource->Fence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	// Update scene-dependent constant buffers
	AnimateMaterials(gt);
	UpdateObjectCBs(gt);
	UpdateMaterialCBs(gt);
	UpdateMainPassCB(gt);
}

/**
 * @brief Draws the game scene.
 *
 * This method draws the game scene by resetting the command list,
 * setting the viewport and scissor rectangles, transitioning the
 * back buffer to the render target state, clearing the back buffer
 * and depth buffer, setting render targets, setting descriptor heaps,
 * setting the root signature, setting the pass constant buffer,
 * drawing the game world, drawing render items, transitioning the
 * back buffer to the present state, executing the command list,
 * presenting the back buffer, and advancing the fence value.
 *
 * @param gt A const reference to a GameTimer object.
 */
void Game::Draw(const GameTimer& gt)
{
	auto cmdListAlloc = mCurrFrameResource->CmdListAlloc;

	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(cmdListAlloc->Reset());

	// Reuse the command list
	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mOpaquePSO.Get()));

	//mScreenViewport.TopLeftX = 0.0f;
	//mScreenViewport.TopLeftY = 0.0f;
	//mScreenViewport.Height = 200;
	//mScreenViewport.Height = 200;

	// Set the viewport and scissor rectangle
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// Indicate a state transition on the resource usage.
	auto transition1 = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	mCommandList->ResourceBarrier(1, &transition1);

	// Clear the back buffer and depth buffer.
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify render targets
	auto dsv = DepthStencilView();

	// Specify the buffers we are going to render to.
	auto buffer = CurrentBackBufferView();
	mCommandList->OMSetRenderTargets(1, &buffer, true, &dsv);

	// Set descriptor heaps
	ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvDescriptorHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// Set root signature
	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	// Set pass constant buffer
	auto passCB = mCurrFrameResource->PassCB->Resource();
	mCommandList->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());

	mStateStack.Draw();

	// Transition the back buffer to the present state
	auto transition2 = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	mCommandList->ResourceBarrier(1, &transition2);

	// Done recording commands.
	ThrowIfFailed(mCommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Present the back buffer
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// Advance the fence value to mark commands up to this fence point.
	mCurrFrameResource->Fence = ++mCurrentFence;

	// Add an instruction to the command queue to set a new fence point. 
	// Because we are on the GPU timeline, the new fence point won't be 
	// set until the GPU finishes processing all the commands prior to this Signal().
	mCommandQueue->Signal(mFence.Get(), mCurrentFence);
}

void Game::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void Game::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void Game::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

		mCamera.Pitch(dy);
		mCamera.RotateY(dx);
	}
	//to do: add another mouse input?

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void Game::OnKeyDown(WPARAM btnState)
{
	mStateStack.HandleEvent(btnState);
}

bool Game::CreateText(const wchar_t* text)
{
	// Create Direct2D factory
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory_);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create Direct2D factory", L"Error", MB_OK);
		return false;
	}

	// Create DirectWrite factory
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory_));
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create DirectWrite factory", L"Error", MB_OK);
		return false;
	}

	wszText_ = text;
	cTextLength_ = (UINT32)wcslen(wszText_);


	// Create text format
	hr = pDWriteFactory_->CreateTextFormat(
		L"Gabriola",                    // Font family
		NULL,                         // No custom font collection
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		72.0f,                           // Font size
		L"en-us",                        // Locale
		&pTextFormat_);                  // Output text format

	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create text format", L"Error", MB_OK);
		return false;
	}
	if (SUCCEEDED(hr))
	{
		hr = pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	}

	if (SUCCEEDED(hr))
	{
		hr = pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	// Center align text horizontally and vertically
	//pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	//pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	// Create Direct2D render target
	hr = pD2DFactory_->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(mhMainWnd, D2D1::SizeU(mClientWidth, mClientHeight)),
		&pRT_);

	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create Direct2D render target", L"Error", MB_OK);
		return false;
	}

	// Create a brush to draw the text (Black brush)
	hr = pRT_->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush_);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create Direct2D solid color brush", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Game::DrawTheText()
{
	OutputDebugStringA("Drawing text...\n");
	if (!pRT_) return;

	// Direct2D and DirectWrite rendering
	// Prepare Direct2D rendering context
	pRT_->BeginDraw();
	D2D1_RECT_F layoutRect = D2D1::RectF(100, 100, 600, 400);

	// Clear the render target with the background color.
	//pRT_->Clear(D2D1::ColorF(D2D1::ColorF::AliceBlue));

	// Draw the text using DirectWrite
	pRT_->DrawText(
		wszText_,                   // Text to display
		cTextLength_,               // Length of text
		pTextFormat_,               // Text format (font, size, etc.)
		D2D1::RectF(100.0f, 100.0f, 500.0f, 200.0f),  // Position of the text
		pBlackBrush_               // Brush to draw with
	);

	// Finish the drawing
	HRESULT hr = pRT_->EndDraw();
	if (FAILED(hr))
	{
		MessageBox(0, L"Direct2D rendering failed", L"Error", MB_OK);
	}
}

#pragma region Keyboard Input
/**
 * @brief Handles keyboard input.
 *
 * Moves the camera based on keyboard input.
 *
 * @param gt A const reference to a GameTimer object.
 */

#pragma region Step 20
 /**
  * @brief Processes input from the player
  *
  * Retrieves the command queue from the world, handles player events, and
  * handles real-time player input.
  */
//void Game::ProcessInput()
//{
//	CommandQueue& commands = mWorld.getCommandQueue();
//	mPlayer.HandleEvent(commands);
//	mPlayer.HandeRealTimeInput(commands);
//}
#pragma endregion

#pragma region Old Keyboard Input
//void Game::OnKeyboardInput(const GameTimer& gt)
//{
//	const float dt = gt.DeltaTime();
//
//	mCamera.GetLook();
//	float tmin = 0;
//	float buffer = 0.5;
//	XMFLOAT3  oppositef3(-1, -1, -1);
//	XMVECTOR opposite = XMLoadFloat3(&oppositef3);
//
//	if (GetAsyncKeyState('W') & 0x8000)
//	{
//		bool hit = false;
//
//		if (!hit)
//		{
//			mCamera.Walk(10.0f * dt);
//
//		}
//	}
//
//	if (GetAsyncKeyState('S') & 0x8000)
//	{
//		bool hit = false;
//		if (!hit)
//		{
//			mCamera.Walk(-10.0f * dt);
//		}
//
//	}
//	if (GetAsyncKeyState('A') & 0x8000)
//	{
//		bool hit = false;
//		if (!hit)
//		{
//			mCamera.Strafe(-10.0f * dt);
//		}
//
//
//	}
//	if (GetAsyncKeyState('D') & 0x8000)
//	{
//		bool hit = false;
//		if (!hit)
//		{
//			mCamera.Strafe(10.0f * dt);
//		}
//	}
//
//
//	mCamera.UpdateViewMatrix();
//}

#pragma endregion

#pragma endregion

/**
 * @brief Updates the camera.
 *
 * Updates the view matrix of the camera.
 *
 */
void Game::UpdateCamera(const GameTimer& gt)
{
	// Convert Spherical to Cartesian coordinates.
	//mEyePos.x = mRadius * sinf(mPhi) * cosf(mTheta);
	//mEyePos.z = mRadius * sinf(mPhi) * sinf(mTheta);
	//mEyePos.y = mRadius * cosf(mPhi);

	//// Build the view matrix.
	//XMVECTOR pos = XMVectorSet(mEyePos.x, mEyePos.y, mEyePos.z, 1.0f);
	//XMVECTOR target = XMVectorZero();
	//XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	//XMStoreFloat4x4(&mView, view);
	mCamera.UpdateViewMatrix();
}

/**
 * @brief Animates the materials.
 *
 * Currently, this method is empty.
 *
 * @param gt A const reference to a GameTimer object.
 */
void Game::AnimateMaterials(const GameTimer& gt)
{
	//No Implementation as of right now.
}

/**
 * @brief Updates the object constant buffers.
 *
 * Updates the object constant buffers for each render item if the
 * constants have changed.
 *
 * @param gt A const reference to a GameTimer object.
 */
void Game::UpdateObjectCBs(const GameTimer& gt)
{
	State* currentState = mStateStack.GetCurrentState();

	auto currObjectCB = mCurrFrameResource->ObjectCB.get();
	for (auto& e : currentState->getRenderItems())
	{
		// Only update the cbuffer data if the constants have changed.  
		if (e->NumFramesDirty > 0)
		{
			XMMATRIX world = XMLoadFloat4x4(&e->World);
			XMMATRIX texTransform = XMLoadFloat4x4(&e->TexTransform);

			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(texTransform));

			currObjectCB->CopyData(e->ObjCBIndex, objConstants);

			// Next FrameResource need to be updated too.
			e->NumFramesDirty--;
		}
	}
}

/**
 * @brief Updates the material constant buffers.
 *
 * Updates the material constant buffers for each material if the
 * constants have changed.
 *
 * @param gt A const reference to a GameTimer object.
 */
void Game::UpdateMaterialCBs(const GameTimer& gt)
{
	auto currMaterialCB = mCurrFrameResource->MaterialCB.get();
	for (auto& e : mMaterials)
	{
		// Only update the cbuffer data if the constants have changed.  If the cbuffer
		// data changes, it needs to be updated for each FrameResource.
		Material* mat = e.second.get();
		if (mat->NumFramesDirty > 0)
		{
			XMMATRIX matTransform = XMLoadFloat4x4(&mat->MatTransform);

			MaterialConstants matConstants;
			matConstants.DiffuseAlbedo = mat->DiffuseAlbedo;
			matConstants.FresnelR0 = mat->FresnelR0;
			matConstants.Roughness = mat->Roughness;
			XMStoreFloat4x4(&matConstants.MatTransform, XMMatrixTranspose(matTransform));

			currMaterialCB->CopyData(mat->MatCBIndex, matConstants);

			// Next FrameResource need to be updated too.
			mat->NumFramesDirty--;
		}
	}
}

/**
 * @brief Updates the main pass constant buffer.
 *
 * Updates the main pass constant buffer with view, projection, and
 * lighting information.
 *
 * @param gt A const reference to a GameTimer object.
 */
void Game::UpdateMainPassCB(const GameTimer& gt)
{
	// Get the view and projection matrices from the camera
	XMMATRIX view = mCamera.GetView();
	XMMATRIX proj = mCamera.GetProj();

	// Calculate the view-projection matrix
	XMMATRIX viewProj = XMMatrixMultiply(view, proj);

	// Calculate the inverse view, projection, and view-projection matrices
	auto detView = XMMatrixDeterminant(view);
	auto detProj = XMMatrixDeterminant(proj);
	auto detViewProj = XMMatrixDeterminant(viewProj);

	XMMATRIX invView = XMMatrixInverse(&detView, view);
	XMMATRIX invProj = XMMatrixInverse(&detProj, proj);
	XMMATRIX invViewProj = XMMatrixInverse(&detViewProj, viewProj);

	// Store the matrices in the main pass constant buffer
	XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));

	// Set other pass constants
	mMainPassCB.EyePosW = mCamera.GetPosition3f();
	mMainPassCB.RenderTargetSize = XMFLOAT2((float)mClientWidth, (float)mClientHeight);
	mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / mClientWidth, 1.0f / mClientHeight);
	mMainPassCB.NearZ = 1.0f;
	mMainPassCB.FarZ = 1000.0f;
	mMainPassCB.TotalTime = gt.TotalTime();
	mMainPassCB.DeltaTime = gt.DeltaTime();
	mMainPassCB.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
	mMainPassCB.Lights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[0].Strength = { 0.6f, 0.6f, 0.6f };
	mMainPassCB.Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[1].Strength = { 0.3f, 0.3f, 0.3f };
	mMainPassCB.Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
	mMainPassCB.Lights[2].Strength = { 0.15f, 0.15f, 0.15f };

	// Copy the pass constant buffer data
	auto currPassCB = mCurrFrameResource->PassCB.get();
	currPassCB->CopyData(0, mMainPassCB);
}

/**
 * @brief Loads the textures.
 *
 * Loads texture resources from files and stores them in the texture map.
 */
void Game::CreateTexture(std::string Name, std::wstring FileName)
{
	auto texture = std::make_unique<Texture>();
	texture->Name = Name;
	texture->Filename = FileName;
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(md3dDevice.Get(),
		mCommandList.Get(), texture->Filename.c_str(),
		texture->Resource, texture->UploadHeap));
	mTextures[texture->Name] = std::move(texture);
}
void Game::LoadTextures()
{
	//Eagle
	CreateTexture("EagleTex", L"../../Textures/Eagle.dds");
	//Raptor
	CreateTexture("RaptorTex", L"../../Textures/Raptor.dds");
	//Desert
	CreateTexture("DesertTex", L"../../Textures/Desert.dds");
	//galaxy
	CreateTexture("GalaxyTex", L"../../Textures/galaxy.dds");
	//title
	CreateTexture("TitleTex", L"../../Textures/galaxy.dds");

}

/**
 * @brief Builds the root signature.
 *
 * Defines the root signature used in the shaders.
 */
void Game::BuildRootSignature()
{
	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[4];

	// Perfomance TIP: Order from most frequent to least frequent.
	slotRootParameter[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);
	slotRootParameter[1].InitAsConstantBufferView(0);
	slotRootParameter[2].InitAsConstantBufferView(1);
	slotRootParameter[3].InitAsConstantBufferView(2);

	auto staticSamplers = GetStaticSamplers();

	// A root signature is an array of root parameters.
	//The Init function of the CD3DX12_ROOT_SIGNATURE_DESC class has two parameters that allow you to
		//define an array of so - called static samplers your application can use.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(4, slotRootParameter,
		(UINT)staticSamplers.size(), staticSamplers.data(),  //6 samplers!
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature.GetAddressOf())));
}

/**
 * @brief Builds the descriptor heaps.
 *
 * Creates and fills the descriptor heaps used for shader resource views (SRVs).
 */

//Once a texture resource is created, we need to create an SRV descriptor to it which we
//can set to a root signature parameter slot for use by the shader programs.
void Game::BuildDescriptorHeaps()
{
	//
	// Create the SRV heap.
	//
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 5;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvDescriptorHeap)));

	//
	// Fill out the heap with actual descriptors.
	//
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	auto EagleTex = mTextures["EagleTex"]->Resource;
	auto RaptorTex = mTextures["RaptorTex"]->Resource;
	auto DesertTex = mTextures["DesertTex"]->Resource;
	auto GalaxyTex = mTextures["GalaxyTex"]->Resource;
	auto TitleTex = mTextures["TitleTex"]->Resource;


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	//This mapping enables the shader resource view (SRV) to choose how memory gets routed to the 4 return components in a shader after a memory fetch.
	//When a texture is sampled in a shader, it will return a vector of the texture data at the specified texture coordinates.
	//This field provides a way to reorder the vector components returned when sampling the texture.
	//D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING  will not reorder the components and just return the data in the order it is stored in the texture resource.
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	srvDesc.Format = EagleTex->GetDesc().Format;

	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	//The number of mipmap levels to view, starting at MostDetailedMip.This field, along with MostDetailedMip allows us to
	//specify a subrange of mipmap levels to view.You can specify - 1 to indicate to view
	//all mipmap levels from MostDetailedMip down to the last mipmap level.

	srvDesc.Texture2D.MipLevels = EagleTex->GetDesc().MipLevels;

	//Specifies the minimum mipmap level that can be accessed. 0.0 means all the mipmap levels can be accessed.
	//Specifying 3.0 means mipmap levels 3.0 to MipCount - 1 can be accessed.
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	md3dDevice->CreateShaderResourceView(EagleTex.Get(), &srvDesc, hDescriptor);

	//Raptor Descriptor
	hDescriptor.Offset(1, mCbvSrvDescriptorSize);
	srvDesc.Format = RaptorTex->GetDesc().Format;
	md3dDevice->CreateShaderResourceView(RaptorTex.Get(), &srvDesc, hDescriptor);

	//Desert Descriptor
	hDescriptor.Offset(1, mCbvSrvDescriptorSize);
	srvDesc.Format = DesertTex->GetDesc().Format;
	md3dDevice->CreateShaderResourceView(DesertTex.Get(), &srvDesc, hDescriptor);

	//Galaxy Descriptor
	hDescriptor.Offset(1, mCbvSrvDescriptorSize);
	srvDesc.Format = GalaxyTex->GetDesc().Format;
	md3dDevice->CreateShaderResourceView(GalaxyTex.Get(), &srvDesc, hDescriptor);

	//Title Descriptor
	hDescriptor.Offset(1, mCbvSrvDescriptorSize);
	srvDesc.Format = TitleTex->GetDesc().Format;
	md3dDevice->CreateShaderResourceView(TitleTex.Get(), &srvDesc, hDescriptor);

}


void Game::BuildShadersAndInputLayout()
{
	mShaders["standardVS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["opaquePS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", nullptr, "PS", "ps_5_1");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//step3
		//The texture coordinates determine what part of the texture gets mapped on the triangles.
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
}

/**
 * @brief Builds the shape geometry.
 *
 * Creates the vertex and index buffers for the shapes used in the scene.
 */
void Game::BuildShapeGeometry()
{
	//mWorld.buildShapeGeometry(md3dDevice, mCommandList, mGeometries);

	GeometryGenerator geoGen;
	GeometryGenerator::MeshData box = geoGen.CreateBox(1, 0, 1, 1);

	SubmeshGeometry boxSubmesh;
	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
	boxSubmesh.StartIndexLocation = 0;
	boxSubmesh.BaseVertexLocation = 0;


	std::vector<Vertex> vertices(box.Vertices.size());

	for (size_t i = 0; i < box.Vertices.size(); ++i)
	{
		vertices[i].Pos = box.Vertices[i].Position;
		vertices[i].Normal = box.Vertices[i].Normal;
		vertices[i].TexC = box.Vertices[i].TexC;
	}

	std::vector<std::uint16_t> indices = box.GetIndices16();

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "boxGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	geo->DrawArgs["box"] = boxSubmesh;

	mGeometries[geo->Name] = std::move(geo);

}

void Game::BuildHillGeometry()
{
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData grid = geoGen.CreateGrid(1, 1, 50, 50);

	SubmeshGeometry gridSubmesh;
	gridSubmesh.IndexCount = (UINT)grid.Indices32.size();
	gridSubmesh.StartIndexLocation = 0;
	gridSubmesh.BaseVertexLocation = 0;


	std::vector<Vertex> vertices(grid.Vertices.size());

	for (size_t i = 0; i < grid.Vertices.size(); ++i)
	{
		auto& p = grid.Vertices[i].Position;
		vertices[i].Pos = p;
		vertices[i].Pos.y = GetHillsHeight(p.x, p.z);
		vertices[i].Normal = GetHillsNormal(p.x, p.z);
		//vertices[i].TexC = grid.Vertices[i].TexC;

		//vertices[k].Pos = grid.Vertices[i].Position;
		//vertices[k].Normal = grid.Vertices[i].Normal;
		vertices[i].TexC = grid.Vertices[i].TexC;
	}

	std::vector<std::uint16_t> indices = grid.GetIndices16();

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "gridGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	geo->DrawArgs["grid"] = gridSubmesh;

	mGeometries[geo->Name] = std::move(geo);

}

/**
 * @brief Builds the pipeline state objects (PSOs).
 *
 * Creates the PSOs for different rendering techniques.
 */
void Game::BuildPSOs()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc;

	//
	// PSO for opaque objects.
	//
	ZeroMemory(&opaquePsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	opaquePsoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	opaquePsoDesc.pRootSignature = mRootSignature.Get();
	opaquePsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mShaders["standardVS"]->GetBufferPointer()),
		mShaders["standardVS"]->GetBufferSize()
	};
	opaquePsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mShaders["opaquePS"]->GetBufferPointer()),
		mShaders["opaquePS"]->GetBufferSize()
	};
	opaquePsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	opaquePsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	opaquePsoDesc.SampleMask = UINT_MAX;
	opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	opaquePsoDesc.NumRenderTargets = 1;
	opaquePsoDesc.RTVFormats[0] = mBackBufferFormat;
	opaquePsoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	opaquePsoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	opaquePsoDesc.DSVFormat = mDepthStencilFormat;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&opaquePsoDesc, IID_PPV_ARGS(&mOpaquePSO)));
}

/**
 * @brief Builds the frame resources.
 *
 * Creates the frame resources used for double buffering.
 */
void Game::BuildFrameResources(int renderItemCount)
{
	for (int i = 0; i < gNumFrameResources; ++i)
	{
		mFrameResources.push_back(std::make_unique<FrameResource>(md3dDevice.Get(),
			1, (UINT)renderItemCount, (UINT)mMaterials.size()));
	}
}

/**
 * @brief Builds the materials.
 *
 * Creates the materials used in the scene.
 */
//step13
void Game::BuildMaterials()
{
	//mWorld.buildMaterials(mMaterials);
	OutputDebugStringA("Building materials...\n");
	mCurrentMaterialCBIndex = 0;
	mCurrentDiffuseSrvHeapIndex = 0;
	CreateMaterials("Eagle", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.05f, 0.05f, 0.05f), 0.2f);
	CreateMaterials("Raptor", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.05f, 0.05f, 0.05f), 0.2f);
	CreateMaterials("Desert", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.05f, 0.05f, 0.05f), 0.2f);
	CreateMaterials("Galaxy", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.05f, 0.05f, 0.05f), 0.2f);
	CreateMaterials("Title", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.05f, 0.05f, 0.05f), 0.2f);
}

void Game::CreateMaterials(std::string Name, XMFLOAT4 DiffuseAlbedo, XMFLOAT3 FresnelR0, float Roughness)
{
	auto material = std::make_unique<Material>();
	material->Name = Name;
	material->MatCBIndex = mCurrentMaterialCBIndex++;
	material->DiffuseSrvHeapIndex = mCurrentDiffuseSrvHeapIndex++;
	material->DiffuseAlbedo = DiffuseAlbedo;
	material->FresnelR0 = FresnelR0;
	material->Roughness = Roughness;

	mMaterials[Name] = std::move(material);
}

void Game::RegisterStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<GameState>(States::Game);

}

void Game::ResetFrameResources()
{
	mFrameResources.clear();
}
/**
 * @brief Builds the render items.
 *
 * Creates the render items used in the scene and adds them to the appropriate lists.
 */
//void Game::BuildRenderItems()
//{
//	mWorld.buildScene();
//
//	// All the render items are opaque.
//	for (auto& e : mAllRitems)
//		mOpaqueRitems.push_back(e.get());
//}
//
///**
// * @brief Draws the render items.
// *
// * Draws the specified render items using the given command list.
// *
// * @param cmdList The command list to use for drawing.
// * @param ritems The render items to draw.
// */
//void Game::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems)
//{
//	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
//	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));
//
//	auto objectCB = mCurrFrameResource->ObjectCB->Resource();
//	auto matCB = mCurrFrameResource->MaterialCB->Resource();
//
//	// For each render item...
//	for (size_t i = 0; i < ritems.size(); ++i)
//	{
//		auto ri = ritems[i];
//
//		auto vbv = ri->Geo->VertexBufferView();
//		auto ibv = ri->Geo->IndexBufferView();
//
//		cmdList->IASetVertexBuffers(0, 1, &vbv);
//		cmdList->IASetIndexBuffer(&ibv);
//		cmdList->IASetPrimitiveTopology(ri->PrimitiveType);
//
//		//step18
//		CD3DX12_GPU_DESCRIPTOR_HANDLE tex(mSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
//		tex.Offset(ri->Mat->DiffuseSrvHeapIndex, mCbvSrvDescriptorSize);
//
//		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + ri->ObjCBIndex * objCBByteSize;
//		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + ri->Mat->MatCBIndex * matCBByteSize;
//
//		//step19
//		cmdList->SetGraphicsRootDescriptorTable(0, tex);
//		cmdList->SetGraphicsRootConstantBufferView(1, objCBAddress);
//		cmdList->SetGraphicsRootConstantBufferView(3, matCBAddress);
//
//		cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
//	}
//}

/**
 * @brief Gets the static samplers.
 *
 * Returns an array of static sampler descriptions.
 *
 * @return An array of const CD3DX12_STATIC_SAMPLER_DESC objects.
 */
//step21
std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> Game::GetStaticSamplers()
{
	// Applications usually only need a handful of samplers.  So just define them all up front
	// and keep them available as part of the root signature.  

	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };
}

float Game::GetHillsHeight(float x, float z)const
{
	return 0.1f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}

XMFLOAT3 Game::GetHillsNormal(float x, float z)const
{
	// n = (-df/dx, 1, -df/dz)
	XMFLOAT3 n(
		-0.03f * z * cosf(0.1f * x) - 0.3f * cosf(0.1f * z),
		1.0f,
		-0.3f * sinf(0.1f * x) + 0.03f * x * sinf(0.1f * z));

	XMVECTOR unitNormal = XMVector3Normalize(XMLoadFloat3(&n));
	XMStoreFloat3(&n, unitNormal);

	return n;
}
