#pragma once
#include <Table/TableInfoBase.h>
#include "../Defines.h"

struct FItemInfo : FTableInfoBase
{
	static constexpr int MAX_EFFECT_COUNT = 10;
	std::string Name;

	std::string IconPath;
	std::string SpritePath;

	int Tier;
	int BasePrice;
	int Limit;

	std::unordered_map<EStat::Type, int> Effects;

	bool Load(std::stringstream& Stream) override
	{
		try
		{
			std::string Segment;

			int IntVal;
			//uint8_t Uint8Val;

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, IntVal)) { assert(false); return false; }
			ID = TableID(IntVal);

			if (!std::getline(Stream, Name, Delim)) { assert(false); return false; }

			if (!std::getline(Stream, IconPath, Delim)) { assert(false); return false; }
			if (!std::getline(Stream, SpritePath, Delim)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, Tier)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, BasePrice)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, Limit)) { assert(false); return false; }
			if (Limit == 0)
			{
				Limit = std::numeric_limits<int>::infinity();
			}

			for (int i = 0; i < MAX_EFFECT_COUNT; i++)
			{
				EStat::Type StatType;
				if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
				if (!TryParse<int>(Segment, IntVal)) { assert(false); return false; }
				StatType = static_cast<EStat::Type>(IntVal);

				if (StatType == EStat::None)
				{
					break;
				}

				if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
				if (!TryParse<int>(Segment, IntVal)) { assert(false); return false; }

				if (!Effects.try_emplace(StatType, IntVal).second) { assert(false); return false; }
			}
		}
		catch (...)
		{
			assert(false);
			return false;
		}

		return true;
	}

	FItemInfo() = default;
	~FItemInfo() = default;
};
