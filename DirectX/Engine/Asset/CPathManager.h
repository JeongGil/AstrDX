#pragma once
#include "../EngineInfo.h"

class CPathManager
{
public:
	static bool Init();
	static void Clear();
	static bool CreatePath(const std::string& Name, const TCHAR* Path, const std::string& BasePathName = "Root");
	static const TCHAR* FindPath(const std::string& Key);

private:
	inline static std::unordered_map<std::string, TCHAR*> Paths;
};

