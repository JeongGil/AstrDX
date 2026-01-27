#pragma once

#include "EngineInfo.h"

class CDevice
{
public:
	/**
	 * @brief Initializes the DirectX device, swap chain, and related resources.
	 *
	 * This method sets up the DirectX environment, including the device, context,
	 * swap chain, render target view, depth stencil view, and viewport. It is
	 * essential for rendering operations in the application.
	 *
	 * @param window The handle to the application window.
	 * @param width The width of the rendering area.
	 * @param height The height of the rendering area.
	 * @param windowMode Specifies whether the application runs in windowed mode
	 *                   (true) or fullscreen mode (false).
	 * @return Returns true if initialization is successful; otherwise, false.
	 */
	bool Init(HWND window, const int width, const int height, const bool windowMode);

	/**
	 * @brief Clears the back buffer to a default color.
	 *
	 * This method resets the back buffer to a predefined color, preparing it
	 * for rendering operations in the current frame. It ensures that any
	 * previous frame's content is removed from the back buffer.
	 *
	 * @note This method is typically called at the beginning of a frame
	 *       rendering process.
	 */
	void ClearBackBuffer();

	/**
	 * @brief Clears the depth stencil buffer with specified values.
	 *
	 * This method resets the depth stencil buffer to the provided depth and stencil
	 * values, ensuring that the depth and stencil data from the previous frame is
	 * cleared. It is typically used at the beginning of a frame rendering process.
	 *
	 * @param depth The depth value to clear the depth buffer to. Default is 1.0f.
	 * @param stencil The stencil value to clear the stencil buffer to. Default is 0.
	 *
	 * @note This method must be called before rendering operations that rely on
	 *       depth or stencil testing.
	 */
	void ClearDepthStencil(float depth = 1.f, UINT8 stencil = 0);

	/**
	 * @brief Sets the render target and depth stencil for the DirectX device.
	 *
	 * This method configures the output merger stage of the rendering pipeline
	 * by binding the render target view and depth stencil view. It ensures that
	 * subsequent rendering operations are directed to the specified render target
	 * and depth stencil.
	 *
	 * @note This method is typically called during the frame initialization process
	 *       to prepare the rendering pipeline for drawing operations.
	 */
	void SetTarget();

	/**
	 * @brief Prepares the rendering pipeline for a new frame.
	 *
	 * This method clears the back buffer and depth stencil, and sets the render
	 * target for the DirectX device. It is typically called at the beginning of
	 * each frame to ensure the rendering pipeline is properly initialized.
	 *
	 * @note This method must be called before any rendering operations in a frame.
	 */
	void BeginRender();

	/**
	 * @brief Finalizes the rendering process for the current frame.
	 *
	 * This method presents the rendered frame to the display by swapping the
	 * back buffer and the front buffer. It is typically called at the end of
	 * the frame rendering process to display the completed frame to the user.
	 *
	 * @note This method must be called after all rendering operations for the
	 *       current frame are completed.
	 */
	void EndRender();

	[[nodiscard]] ID3D11Device* GetDevice() const
	{
		return Device;
	}

	[[nodiscard]] ID3D11DeviceContext* GetContext() const
	{
		return Context;
	}

	[[nodiscard]] const FResolution& GetResolution() const
	{
		return Resolution;
	}

	[[nodiscard]] bool GetWindowMode() const
	{
		return bWindowMode;
	}

	[[nodiscard]] FVector2 GetResolutionRatio() const
	{
		RECT WindowRC;
		GetClientRect(window, &WindowRC);

		FVector2 Ratio;
		Ratio.x = Resolution.Width / static_cast<float>(WindowRC.right - WindowRC.left);
		Ratio.y = Resolution.Height / static_cast<float>(WindowRC.bottom - WindowRC.top);

		return Ratio;
	}

private:
	// It is mainly used when creating resources.
	ID3D11Device* Device = nullptr;
	ID3D11DeviceContext* Context = nullptr;
	IDXGISwapChain* SwapChain = nullptr;
	ID3D11RenderTargetView* TargetView = nullptr;
	ID3D11DepthStencilView* DepthView = nullptr;

	FResolution Resolution;

	bool bWindowMode = false;

	HWND window = nullptr;

public:
	static CDevice* GetInst()
	{
		if (Inst == nullptr)
		{
			Inst = new CDevice;
		}

		return Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(Inst)
	}

private:
	CDevice();
	~CDevice();

	inline static CDevice* Inst = nullptr;
};

