#pragma once
#include <EngineInfo.h>

inline std::string TCharToStdString(const TCHAR* tcharStr)
{
	if (!tcharStr) return "";

#ifdef UNICODE
	int size = WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, nullptr, 0, nullptr, nullptr);
	if (size <= 0) return "";

	std::string result(size - 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, &result[0], size, nullptr, nullptr);
	return result;
#else
	return std::string(tcharStr);
#endif
}
