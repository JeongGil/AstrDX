#pragma once

#include "CThreadBase.h"

class CThreadManager
{
private:
	CThreadManager();
	~CThreadManager();

	std::unordered_map<std::string, std::shared_ptr<CThreadBase>> Threads;

public:
	bool Init();
	bool DeleteThread(const std::string& Key);

	template <typename T>
	std::weak_ptr<T> FindThread(const std::string& Key)
	{
		auto It = Threads.find(Key);

		if (It == Threads.end())
		{
			return {};
		}

		return std::dynamic_pointer_cast<T>(It->second);
	}

	template <typename T>
	std::shared_ptr<T> Create(const std::string& Key, bool Pause = false)
	{
		auto It = Threads.find(Key);
		if (It != Threads.end())
		{
			return std::dynamic_pointer_cast<T>(It->second);
		}

		std::shared_ptr<CThreadBase> Thread;
		Thread.reset(new T);

		Thread->SetKey(Key);

		if (!Thread->Init(Pause))
		{
			return {};
		}

		Threads.emplace(Key, Thread);

		return std::dynamic_pointer_cast<T>(Thread);
	}

	static CThreadManager& GetInst()
	{
		static CThreadManager Inst;
		return Inst;
	}

	static void Clear()
	{
		for (auto& Thread : GetInst().Threads | std::views::values)
		{
			Thread->Exit();
		}

		GetInst().Threads.clear();
	}
};

