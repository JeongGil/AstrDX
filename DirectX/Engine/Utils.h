#pragma once
#include <memory>

#include "Sync.h"

template <typename T>
bool IsSameTarget(const std::weak_ptr<T>& a, const std::weak_ptr<T>& b)
{
	return !a.owner_before(b) && !b.owner_before(a);
}

class CIDMaker
{
	inline static CRITICAL_SECTION Crt;
	inline static size_t NextID{ 0 };
	inline static std::unordered_map<std::string, size_t> IDs;

public:
	static size_t GetID(const std::string& Key)
	{
		CSync _(&Crt);

		auto It = IDs.find(Key);
		if (It == IDs.end())
		{
			auto [it, result] = IDs.emplace(Key, NextID++);

			return it->second;
		}

		return It->second;
	}
};
