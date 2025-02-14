#include "Game.hpp"

/**
 * @brief Entry point for the Windows application.
 *
 * This function serves as the main entry point for the Windows application.
 * It initializes the game, runs it, and handles any exceptions that may occur.
 *
 * @param hInstance Handle to the current instance of the application.
 * @param prevInstance Handle to the previous instance of the application (always NULL for Win32 apps).
 * @param cmdLine Command line arguments as a single string.
 * @param showCmd Control how the window is to be shown (e.g., maximized, minimized).
 * @return int Returns 0 if the game exits normally, or if initialization fails.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		// Create and initialize the game
		Game theApp(hInstance);
		if (!theApp.Initialize())
			return 0;

		// Run the game
		return theApp.Run();
	}
	catch (DxException& e)
	{
		// Handle any DirectX exceptions
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}

