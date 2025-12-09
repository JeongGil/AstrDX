#pragma once

#include <Windows.h>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include <string>
#include <functional>

// check memory leak
#include <crtdbg.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#include "resource.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#define SAFE_DELETE(p)       { if(p) { delete (p); (p)=nullptr; } }

struct FResolution
{
	int Width = 0;
	int Height = 0;
};
