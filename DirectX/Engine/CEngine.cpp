#include "CEngine.h"

#include "CCollisionInfoManager.h"
#include "CDevice.h"
#include "CTimer.h"
#include "Asset/CAssetManager.h"
#include "Asset/Mesh/CMesh.h"
#include "Asset/Mesh/CMeshManager.h"
#include "Asset/Shader/CShaderManager.h"
#include "World/CWorldManager.h"
#include "Asset/Shader/CCBufferTransform.h"
#include "CObject.h"
#include "Object/CGameObject.h"
#include "Component/CMeshComponent.h"
#include "Component/CCameraComponent.h"
#include "Render/CRenderManager.h"

void CEngine::InitCDO()
{
	CObject::CreateCDO<CGameObject>();
	CObject::CreateCDO<CSceneComponent>();
	CObject::CreateCDO<CMeshComponent>();
	CObject::CreateCDO<CCameraComponent>();
}

bool CEngine::Init(const HINSTANCE hInstance, const TCHAR* WindowName, const int IconID, const int SmallIconID,
                   const int Width, const int Height, const bool WindowMode)
{
	std::random_device RD;
	MT = std::mt19937_64(RD());

	hInst = hInstance;

	InitRegisterClass(WindowName, IconID, SmallIconID);

	InitCreateWindow(WindowName, Width, Height);

	if (!CDevice::GetInst()->Init(hWnd, Width, Height, WindowMode))
	{
		return false;
	}

	if (!CRenderManager::GetInst()->Init())
	{
		return false;
	}

	if (!CCollisionInfoManager::GetInst()->Init())
	{
		return false;
	}

	if (!CAssetManager::GetInst()->Init())
	{
		return false;
	}

	CMeshComponent::CreateEmptyAnimationCBuffer();

	Setting->Init();

	if (!CWorldManager::GetInst()->Init())
	{
		return false;
	}

	return true;
}

int CEngine::Run()
{
	MSG msg{};

	while (bLoop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
		else
		{
			Loop();
		}
	}

	return static_cast<int>(msg.wParam);
}

void CEngine::Loop()
{
	Update(CTimer::Update());

	Render();
}

void CEngine::Update(const float deltaTime)
{
	CWorldManager::GetInst()->Update(deltaTime);
	CWorldManager::GetInst()->PostUpdate(deltaTime);
}

void CEngine::Render()
{
	CDevice::GetInst()->BeginRender();

	//CWorldManager::GetInst()->Render();
	CRenderManager::GetInst()->Render();

	CDevice::GetInst()->EndRender();
}

void CEngine::InitRegisterClass(const TCHAR* WindowName, const int IconID, const int SmallIconID) const
{
	WNDCLASSEXW wc
	{
		.cbSize = sizeof(WNDCLASSEXW),
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = CEngine::WndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hInst,
		.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IconID)),
		.hCursor = LoadCursor(nullptr, IDC_ARROW),
		.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
		.lpszMenuName = nullptr,
		.lpszClassName = WindowName,
		.hIconSm = LoadIcon(hInst, MAKEINTRESOURCE(SmallIconID))
	};

	RegisterClassExW(&wc);
}

bool CEngine::InitCreateWindow(const TCHAR* WindowName, const int Width, const int Height)
{
	hWnd = CreateWindowW(WindowName, WindowName,
		WS_OVERLAPPEDWINDOW, Width, 0, Height, 0,
		nullptr, nullptr, hInst, nullptr);

	if (!hWnd)
	{
		return false;
	}

	RECT WindowRC{ 0, 0, Width, Height };
	AdjustWindowRect(&WindowRC, WS_OVERLAPPEDWINDOW, FALSE);

	SetWindowPos(hWnd, HWND_TOPMOST,
		0, 0, WindowRC.right - WindowRC.left, WindowRC.bottom - WindowRC.top,
		SWP_NOMOVE | SWP_NOZORDER);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return true;
}

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
		{
			bLoop = false;
			PostQuitMessage(0);
			break;
		}
		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return 0;
}

CEngine::CEngine()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// put a breakpoint on memory allocation number from _CrtSetDbgFlag.
	//_crtBreakAlloc(151);

	Setting.reset(new CEngineSetting);
}

CEngine::~CEngine()
{
	CMeshComponent::ClearEmptyAnimationCBuffer();

	CWorldManager::DestroyInst();

	CObject::ClearCDO();

	CRenderManager::DestroyInst();

	CCollisionInfoManager::DestroyInst();

	CAssetManager::DestroyInst();

	CDevice::DestroyInst();
}
