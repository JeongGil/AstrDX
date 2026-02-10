#pragma once
#include <memory>
#include <mutex>
#include <unordered_map>
#include <string>

template <typename T>
bool IsSameTarget(const std::weak_ptr<T>& a, const std::weak_ptr<T>& b)
{
	return !a.owner_before(b) && !b.owner_before(a);
}

class CIDMaker
{
	// CRITICAL_SECTION 대신 std::mutex 사용
	inline static std::mutex Mtx;
	inline static size_t NextID{ 0 };
	inline static std::unordered_map<std::string, size_t> IDs;

public:
	static size_t GetID(const std::string& Key)
	{
		// std::lock_guard를 사용하면 생성 시 lock, 소멸 시 unlock이 자동으로 처리됩니다.
		std::lock_guard<std::mutex> lock(Mtx);

		auto It = IDs.find(Key);
		if (It == IDs.end())
		{
			auto [it, result] = IDs.emplace(Key, NextID++);
			return it->second;
		}

		return It->second;
	}
};
