#include <CEngine.h>

#ifdef _DEBUG
// link debug lib
#pragma comment(lib, "Engine_Debug.lib")

#else
#pragma comment(lib, "Engine.lib")

#endif


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (!CEngine::GetInst()->Init(hInstance, TEXT("GameClient"),
		IDI_ICON1, IDI_ICON1,
		1280, 720, true))
	{
		CEngine::DestroyInst();
	}

	int ret = CEngine::GetInst()->Run();

	CEngine::DestroyInst();

	return ret;
}
