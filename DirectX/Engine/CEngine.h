#pragma once

#include "CEngineSetting.h"
#include "CObject.h"
#include "EngineInfo.h"

class CEngine
{
public:
	template <typename T>
	static void CreateCDO()
	{
		CObject::CreateCDO<T>();
	}

	template <typename T>
	std::weak_ptr<T> CreateEngineSetting()
	{
		Setting.reset(new T);

		return std::dynamic_pointer_cast<T>(Setting);
	}

	std::mt19937_64& GetMT() { return MT; }

	[[nodiscard]] HINSTANCE GetWindowInstance() const { return hInst; }
	[[nodiscard]] HWND GetWindowHandle() const { return hWnd; }

private:
	void InitCDO();

public:
	/**
	 * @brief Initializes the engine and its essential components.
	 *
	 * This method sets up the engine by registering the window class, creating the application window,
	 * and initializing core subsystems such as the Device, asset manager, and world manager.
	 *
	 * @param hInstance The handle to the current instance of the application.
	 * @param WindowName The name of the application window.
	 * @param IconID The resource ID of the main application icon.
	 * @param SmallIconID The resource ID of the small application icon.
	 * @param Width The width of the application window.
	 * @param Height The height of the application window.
	 * @param WindowMode Specifies whether the application runs in windowed mode (true) or fullscreen mode (false).
	 * @return true if the initialization is successful; otherwise, false.
	 */
	bool Init(HINSTANCE hInstance, const TCHAR* WindowName, int IconID, int SmallIconID, int Width, int Height, bool WindowMode);

	/**
	 * @brief Executes the main loop of the engine.
	 *
	 * This method runs the engine's main loop, which processes system messages
	 * and executes the game loop. It continues to run until the application
	 * signals the termination of the loop.
	 *
	 * @return The exit code of the application, typically the value of `msg.wParam`.
	 */
	int Run();

private:
	/**
	 * @brief Executes the main game loop logic.
	 *
	 * This method is called repeatedly during the engine's main loop to perform
	 * game-specific updates and rendering. It ensures that the game state is updated
	 * based on the elapsed time and that the scene is rendered to the screen.
	 *
	 * The `Loop` method typically involves:
	 * - Updating the game state using the `Update` method.
	 * - Rendering the updated scene using the `Render` method.
	 */
	void Loop();

	void Update(float deltaTime);

	/**
	 * @brief Renders the current frame of the application.
	 *
	 * This method is responsible for executing the rendering process for the current frame.
	 * It begins the rendering sequence, delegates the rendering of the world to the `CWorldManager`,
	 * and finalizes the rendering process. It ensures that all rendering operations are performed
	 * in the correct order and that the frame is properly displayed.
	 *
	 * The rendering process typically involves:
	 * - Starting the rendering sequence using `CDevice::BeginRender`.
	 * - Rendering the world using `CWorldManager::Render`.
	 * - Ending the rendering sequence using `CDevice::EndRender`.
	 */
	void Render();

private:
	/**
	 * @brief Registers the window class for the application.
	 *
	 * This method sets up the window class by defining its properties, such as the window procedure,
	 * instance handle, icons, cursor, background, and class name. It ensures that the application
	 * window can be created and displayed properly.
	 *
	 * @param WindowName The name of the window class to be registered.
	 * @param IconID The resource ID of the main application icon.
	 * @param SmallIconID The resource ID of the small application icon.
	 */
	void InitRegisterClass(const TCHAR* WindowName, int IconID, int SmallIconID) const;

	/**
	 * @brief Creates and initializes the application window.
	 *
	 * This method creates the main application window with the specified dimensions and properties.
	 * It adjusts the window size to account for the window frame, sets the window position, and
	 * ensures the window is displayed and updated properly.
	 *
	 * @param WindowName The name of the application window.
	 * @param Width The desired width of the application window.
	 * @param Height The desired height of the application window.
	 * @return true if the window is successfully created and initialized; otherwise, false.
	 */
	bool InitCreateWindow(const TCHAR* WindowName, int Width, int Height);


	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE hInst = nullptr;
	HWND hWnd = nullptr;

	std::shared_ptr<CEngineSetting> Setting;

	inline static bool bLoop = true;

	std::mt19937_64 MT;

public:
	static CEngine* GetInst()
	{
		if (inst == nullptr)
		{
			inst = new CEngine;
		}

		return inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(inst)
	}

private:
	CEngine();
	~CEngine();

	inline static CEngine* inst = nullptr;
};
