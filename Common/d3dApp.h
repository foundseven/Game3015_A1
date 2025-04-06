

#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "d3dUtil.h"
#include "GameTimer.h"

// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

/**
 * @brief Direct3D 12 application base class
 *
 * Provides core functionality for Direct3D 12 applications including:
 * - Window management
 * - Direct3D device and swap chain initialization
 * - Command queue/allocator management
 * - Resource descriptor heaps
 * - Main application loop
 */
class D3DApp
{
protected:
    /**
     * @brief Constructs a D3DApp instance
     * @param hInstance Handle to the application instance
     */
    D3DApp(HINSTANCE hInstance);
    D3DApp(const D3DApp& rhs) = delete;
    D3DApp& operator=(const D3DApp& rhs) = delete;

    /**
     * @brief Destructor - releases Direct3D resources
     */
    virtual ~D3DApp();

public:
    //-------------------------------------------------------------------------
    // Core Application Interface
    //-------------------------------------------------------------------------
    
    /**
     * @brief Gets the singleton application instance
     * @return Pointer to the D3DApp instance
     */
    static D3DApp* GetApp();
    
	/**
	 * @brief Gets the application instance handle
	 * @return Handle to the application instance
	 */
	HINSTANCE AppInst()const;

    /**
     * @brief Gets the main window handle
     * @return Handle to the application's main window
     */
	HWND      MainWnd()const;

    /**
     * @brief Gets the aspect ratio of the client area
     * @return Aspect ratio (width/height)
     */
	float     AspectRatio()const;

    //-------------------------------------------------------------------------
    // MSAA Configuration
    //-------------------------------------------------------------------------
    
    /**
     * @brief Gets current 4x MSAA state
     * @return True if 4x MSAA is enabled
     */
    bool Get4xMsaaState()const;

    /**
     * @brief Enables/disables 4x MSAA
     * @param value True to enable 4x MSAA, false to disable
     */
    void Set4xMsaaState(bool value);

    //-------------------------------------------------------------------------
    // Application Execution
    //-------------------------------------------------------------------------
    
    /**
     * @brief Runs the main application loop
     * @return Application exit code
     */
	int Run();

    /**
    * @brief Initializes application resources
    * @return True if initialization succeeded
    */
    virtual bool Initialize();

    /**
     * @brief Handles Windows messages
     * @param hwnd Window handle
     * @param msg Message ID
     * @param wParam Message parameter
     * @param lParam Message parameter
     * @return Message processing result
     */
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
    //-------------------------------------------------------------------------
    // Resource Management
    //-------------------------------------------------------------------------

    /**
     * @brief Creates RTV and DSV descriptor heaps
     */
    virtual void CreateRtvAndDsvDescriptorHeaps();

    /**
     * @brief Handles window resize events
     */
    virtual void OnResize();

    /**
     * @brief Pure virtual update method for frame logic
     * @param gt Game timer reference
     */
    virtual void Update(const GameTimer& gt) = 0;

    /**
     * @brief Pure virtual render method
     * @param gt Game timer reference
     */
    virtual void Draw(const GameTimer& gt) = 0;

    //-------------------------------------------------------------------------
    // Input Handling (Override in derived classes)
    //-------------------------------------------------------------------------

    /**
     * @brief Handles mouse button down events
     * @param btnState Mouse button state flags
     * @param x Cursor X position
     * @param y Cursor Y position
     */
    virtual void OnMouseDown(WPARAM btnState, int x, int y) {}

    /**
     * @brief Handles mouse button up events
     * @param btnState Mouse button state flags
     * @param x Cursor X position
     * @param y Cursor Y position
     */
    virtual void OnMouseUp(WPARAM btnState, int x, int y) {}

    /**
     * @brief Handles mouse movement events
     * @param btnState Mouse button state flags
     * @param x Cursor X position
     * @param y Cursor Y position
     */
    virtual void OnMouseMove(WPARAM btnState, int x, int y) {}

    /**
     * @brief Handles key down events
     * @param btnState Key state flags
     */
    virtual void OnKeyDown(WPARAM btnState) {}

protected:
    //-------------------------------------------------------------------------
    // Initialization Helpers
    //-------------------------------------------------------------------------

    /**
     * @brief Initializes main application window
     * @return True if window creation succeeded
     */
    bool InitMainWindow();

    /**
     * @brief Initializes Direct3D resources
     * @return True if Direct3D initialization succeeded
     */
    bool InitDirect3D();

    /**
     * @brief Creates command queue and command list objects
     */
    void CreateCommandObjects();

    /**
     * @brief Creates swap chain for window
     */
    void CreateSwapChain();

    //-------------------------------------------------------------------------
    // Command Queue Management
    //-------------------------------------------------------------------------

    /**
     * @brief Flushes the command queue
     * @details Waits for GPU to finish executing all commands
     */
    void FlushCommandQueue();

    //-------------------------------------------------------------------------
    // Resource Accessors
    //-------------------------------------------------------------------------

    /**
     * @brief Gets current back buffer resource
     * @return Pointer to current back buffer texture
     */
    ID3D12Resource* CurrentBackBuffer()const;

    /**
     * @brief Gets current back buffer RTV descriptor
     * @return CPU descriptor handle for current back buffer
     */
    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;

    /**
     * @brief Gets depth stencil view descriptor
     * @return CPU descriptor handle for depth/stencil buffer
     */
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

    //-------------------------------------------------------------------------
    // Debug Utilities
    //-------------------------------------------------------------------------

    /**
     * @brief Calculates and displays frame statistics
     */
    void CalculateFrameStats();

    /**
     * @brief Logs available GPU adapters
     */
    void LogAdapters();

    /**
     * @brief Logs outputs for a specific adapter
     * @param adapter DXGI adapter to query
     */
    void LogAdapterOutputs(IDXGIAdapter* adapter);

    /**
     * @brief Logs display modes for an output
     * @param output DXGI output to query
     * @param format Format to enumerate modes for
     */
    void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

protected:

    //-------------------------------------------------------------------------
    // Application State
    //-------------------------------------------------------------------------

    static D3DApp* mApp;           ///< Singleton application instance

    HINSTANCE mhAppInst = nullptr; ///< Application instance handle
    HWND mhMainWnd = nullptr;      ///< Main window handle

    bool mAppPaused = false;       ///< Application pause state
    bool mMinimized = false;       ///< Window minimized state
    bool mMaximized = false;       ///< Window maximized state
    bool mResizing = false;        ///< Window resize in progress
    bool mFullscreenState = false; ///< Fullscreen enabled state

    //-------------------------------------------------------------------------
    // MSAA Configuration
    //-------------------------------------------------------------------------

    bool m4xMsaaState = false;     ///< 4x MSAA enabled state
    UINT m4xMsaaQuality = 0;       ///< Quality level for 4x MSAA

    //-------------------------------------------------------------------------
    // Timing
    //-------------------------------------------------------------------------

    GameTimer mTimer;              ///< Game timing utility

    //-------------------------------------------------------------------------
    // Direct3D Core Objects
    //-------------------------------------------------------------------------

    Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
    Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
    Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;

    //-------------------------------------------------------------------------
    // Synchronization
    //-------------------------------------------------------------------------

    Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
    UINT64 mCurrentFence = 0;      ///< Current fence value for GPU synchronization

    //-------------------------------------------------------------------------
    // Command Management
    //-------------------------------------------------------------------------

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

    //-------------------------------------------------------------------------
    // Swap Chain Resources
    //-------------------------------------------------------------------------

    static const int SwapChainBufferCount = 2;
    int mCurrBackBuffer = 0;
    Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
    Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

    //-------------------------------------------------------------------------
    // Descriptor Heaps
    //-------------------------------------------------------------------------

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

    //-------------------------------------------------------------------------
    // Viewport/Scissor
    //-------------------------------------------------------------------------

    D3D12_VIEWPORT mScreenViewport; ///< Current viewport configuration
    D3D12_RECT mScissorRect;        ///< Current scissor rectangle

    //-------------------------------------------------------------------------
    // Descriptor Sizes
    //-------------------------------------------------------------------------

    UINT mRtvDescriptorSize = 0;    ///< RTV descriptor size in bytes
    UINT mDsvDescriptorSize = 0;    ///< DSV descriptor size in bytes
    UINT mCbvSrvUavDescriptorSize = 0; ///< CBV/SRV/UAV descriptor size

    //-------------------------------------------------------------------------
    // Configurable Parameters
    //-------------------------------------------------------------------------

    std::wstring mMainWndCaption = L"d3d App"; ///< Window caption text
    D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    int mClientWidth = 800;         ///< Initial client width
    int mClientHeight = 600;        ///< Initial client height
};

