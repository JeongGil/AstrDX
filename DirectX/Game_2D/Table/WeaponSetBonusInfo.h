#pragma once
#include <Table/TableInfoBase.h>

#include "../Defines.h"

struct FBonusStat
{
	EStat::Type StatType{};
	int StatValue{};
};

struct FWeaponSetBonusInfo : FTableInfoBase
{
	std::string Name;

	EWeaponType::Type WeaponType;

	std::array<FBonusStat, INVENTORY_MAX_WEAPON - 1> Bonus1;
	std::array<FBonusStat, INVENTORY_MAX_WEAPON - 1> Bonus2;


	bool Load(std::stringstream& Stream) override
	{
		try
		{
			std::string Segment;

			int IntVal;
			uint8_t Uint8Val;
			uint32_t Uint32Val;

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, IntVal)) { assert(false); return false; }
			ID = TableID(IntVal);

			if (!std::getline(Stream, Name, Delim)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<uint32_t>(Segment, Uint32Val)) { assert(false); return false; }
			WeaponType = static_cast<EWeaponType::Type>(Uint32Val);

			for (auto& Bonus : Bonus1)
			{
				if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
				if (!TryParse<uint8_t>(Segment, Uint8Val)) { assert(false); return false; }
				Bonus.StatType = static_cast<EStat::Type>(Uint8Val);

				if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
				if (!TryParse<int>(Segment, Bonus.StatValue)) { assert(false); return false; }
			}

			for (auto& Bonus : Bonus2)
			{
				if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
				if (!TryParse<uint8_t>(Segment, Uint8Val)) { assert(false); return false; }
				Bonus.StatType = static_cast<EStat::Type>(Uint8Val);

				if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
				if (!TryParse<int>(Segment, Bonus.StatValue)) { assert(false); return false; }
			}
		}
		catch (...)
		{
			assert(false); 
			return false;
		}

		return true;
	}

	FWeaponSetBonusInfo() = default;
	~FWeaponSetBonusInfo() = default;
};
