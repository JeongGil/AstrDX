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
			UINT8 Uint8Val;
			UINT32 Uint32Val;

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, IntVal)) { return false; }
			ID = TableID(IntVal);

			if (!std::getline(Stream, Name, Delim)) { return false; }

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<UINT32>(Segment, Uint32Val)) { return false; }
			WeaponType = static_cast<EWeaponType::Type>(Uint32Val);

			for (auto& Bonus : Bonus1)
			{
				if (!std::getline(Stream, Segment, Delim)) { return false; }
				if (!TryParse<UINT8>(Segment, Uint8Val)) { return false; }
				Bonus.StatType = static_cast<EStat::Type>(Uint8Val);

				if (!std::getline(Stream, Segment, Delim)) { return false; }
				if (!TryParse<int>(Segment, Bonus.StatValue)) { return false; }
			}

			for (auto& Bonus : Bonus2)
			{
				if (!std::getline(Stream, Segment, Delim)) { return false; }
				if (!TryParse<UINT8>(Segment, Uint8Val)) { return false; }
				Bonus.StatType = static_cast<EStat::Type>(Uint8Val);

				if (!std::getline(Stream, Segment, Delim)) { return false; }
				if (!TryParse<int>(Segment, Bonus.StatValue)) { return false; }
			}
		}
		catch (...)
		{
			return false;
		}

		return true;
	}

	FWeaponSetBonusInfo() = default;
	~FWeaponSetBonusInfo() = default;
};
