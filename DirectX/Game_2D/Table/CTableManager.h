#pragma once
#include <EngineInfo.h>

#include "../Strings.h"

class CTableManager
{
public:
	static CTableManager& GetInst()
	{
		static CTableManager Inst;
		return Inst;
	}

	bool Init();
	void LoadTables();

private:
	static std::string GetFilePath(const TCHAR* FileName, const std::string& PathKey = Key::Path::Table);

private:
	CTableManager() = default;
	~CTableManager() = default;
};

