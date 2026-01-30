#pragma once
#include <fstream>
#include <functional>
#include <ranges>
#include <sstream>
#include <cassert>

#include "TableInfoBase.h"
#include "../EngineInfo.h"

template <typename Derived, typename T>
class TableBase abstract
{
	static_assert(std::is_base_of<FTableInfoBase, T>::value, "T must inherit from FTableInfoBase.");

public:
	static Derived& GetInst()
	{
		static Derived Instance;
		return Instance;
	}

	bool Load(const TCHAR* FilePath)
	{
		std::ifstream File(FilePath);
		return ProcessFileContent(File);
	}

	bool Load(const std::string& FilePath)
	{
		std::ifstream File(FilePath);
		return ProcessFileContent(File);
	}

	void Clear()
	{
		for (auto& Item : Items | std::views::values)
		{
			delete Item;
		}

		Items.clear();
	}

	bool Contains(TableID ID) const
	{
		return Items.contains(ID);
	}

	const T* Get(TableID ID) const
	{
		auto It = Items.find(ID);
		if (It != Items.end())
		{
			return It->second;
		}
		else
		{
			return nullptr;
		}
	}

	bool TryGet(TableID ID, T*& OutItem) const
	{
		auto It = Items.find(ID);
		if (It != Items.end())
		{
			OutItem = It->second;
			return true;
		}
		else
		{
			OutItem = nullptr;
			return false;
		}
	}

	using MapIterator = std::unordered_map<TableID, T*>::iterator;
	MapIterator Begin()
	{
		return Items.begin();
	}

	MapIterator End()
	{
		return Items.end();
	}

	MapIterator Find(TableID ID)
	{
		return Items.find(ID);
	}

private:
	bool ProcessFileContent(std::ifstream& File)
	{
		if (!File.is_open())
		{
			return false;
		}

		Clear();

		std::string Line;

		// Skip header line.
		std::getline(File, Line);

		while (std::getline(File, Line))
		{
			if (Line.empty())
			{
				continue;
			}

			std::stringstream SS(Line);
			auto Info = new T;

			if (Info->Load(SS))
			{
#ifdef _DEBUG
				if (!Items.try_emplace(Info->Key, Info).second)
				{
					char Test[256] = {};
					sprintf_s(Test, "Key : %d duplicated.\n", static_cast<int>(Info->Key));

					OutputDebugStringA(Test);
				}
#else
				Items.emplace(Info->Key, Info)
#endif
			}
			else
			{
				delete Info;

#ifdef  _DEBUG
				assert(false);
#endif				
			}
		}

		return true;
	}

protected:
	TableBase() = default;

	virtual ~TableBase()
	{
		Clear();
	}

protected:
	std::unordered_map<TableID, T*> Items;
};
