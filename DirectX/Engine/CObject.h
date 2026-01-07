#pragma once

#include "EngineInfo.h"

class CObject : public std::enable_shared_from_this<CObject>
{
	friend class CEngine;

public:
	static void ClearCDO()
	{
		CDOs.clear();
	}

protected:
	template <typename T>
	static std::weak_ptr<T> CreateCDO()
	{
		std::type_index Key(typeid(T));
		if (CDOs.contains(Key))
		{
			return std::weak_ptr<T>();
		}

		std::shared_ptr<T> CDO(new T);

		CDOs.emplace(Key, CDO);

		return std::dynamic_pointer_cast<T>(CDO);
	}

	template <typename T>
	static std::weak_ptr<T> FindCDO()
	{
		std::type_index Key(typeid(T));
		auto It = CDOs.find(Key);
		if (It == CDOs.end())
		{
			return std::weak_ptr<T>();
		}
		else
		{
			return std::dynamic_pointer_cast<T>(It->second);
		}
	}

	static std::weak_ptr<CObject> FindCDO(std::type_index Key)
	{
		auto It = CDOs.find(Key);
		if (It == CDOs.end())
		{
			return std::weak_ptr<CObject>();
		}
		else
		{
			return It->second;
		}
	}

private:
	inline static std::unordered_map<std::type_index, std::shared_ptr<CObject>> CDOs;

public:
	CObject() = default;
	CObject(const CObject& other) = default;
	CObject(CObject&& other) noexcept = default;
	CObject& operator=(const CObject& other) = default;
	CObject& operator=(CObject&& other) noexcept = default;
	virtual ~CObject() = 0;
};

