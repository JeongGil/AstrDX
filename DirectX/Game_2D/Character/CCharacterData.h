#pragma once
#include "../Defines.h"
#include <EngineInfo.h>

class CCharacterData
{
	const FStat& GetStat(EStat::Type Type) const
	{
		assert(Stats.contains(Type));
		return Stats.at(Type);
	}

	void SetStat(const FStat& Stat)
	{
		Stats[Stat.Type] = Stat;
	}

	void SetStat(EStat::Type Type, int Value)
	{
		auto It = Stats.find(Type);
		if (It != Stats.end())
		{
			It->second.InnerValue = Value;
		}
	}

	void AddStat(EStat::Type Type, int Value)
	{
		auto It = Stats.find(Type);
		if (It != Stats.end())
		{
			It->second.InnerValue += Value;
		}
	}
private:
	std::unordered_map<EStat::Type, FStat> Stats;

public:
	static CCharacterData& GetInst()
	{
		static CCharacterData Inst;
		return Inst;
	}

private:
	CCharacterData() = default;
	~CCharacterData() = default;
};
