#pragma once
#include <EngineInfo.h>
#include <Table/TableInfoBase.h>

#include "../Defines.h"

struct FDamageScale
{
	EStat::Type DamageType = EStat::None;
	int ScalePercent = 0;
};

struct FSpecialEffect
{
	std::array<int, 10> Params;
};

struct FWeaponInfo
	: FTableInfoBase
{
	friend class WeaponTable;

	std::string Name;
	EWeaponType::Type Type;
	
	int BaseDamage;
	FDamageScale DamageScale1;
	FDamageScale DamageScale2;
	
	int CoolTimeMS;
	
	int CritChancePercent;
	int CritDamagePercent;

	int Range;
	int Knockback;
	int LifeStealPercent;

	FSpecialEffect SpecialEffect;
	
	int BasePrice;

	std::string IconPath;
	std::string SpritePath1;
	std::string SpritePath2;

	bool Load(std::stringstream& Stream) override
	{
		try
		{
			std::string Segment;

			int IntVal;			
			uint8_t Uint8Val;

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, IntVal)) { return false; }
			ID = TableID(IntVal);

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			Name = Segment;

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, BaseDamage)) { return false; }

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<uint8_t>(Segment, Uint8Val)) { return false; }
			DamageScale1.DamageType = static_cast<EStat::Type>(Uint8Val);

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, DamageScale1.ScalePercent)) { return false; }

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<uint8_t>(Segment, Uint8Val)) { return false; }
			DamageScale2.DamageType = static_cast<EStat::Type>(Uint8Val);

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, DamageScale2.ScalePercent)) { return false; }

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, CoolTimeMS)) { return false; }

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, CritChancePercent)) { return false; }

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, CritDamagePercent)) { return false; }

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, Range)) { return false; }

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, Knockback)) { return false; }

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, LifeStealPercent)) { return false; }

			for (auto& Param : SpecialEffect.Params)
			{
				if (!std::getline(Stream, Segment, Delim)) { return false; }
				if (!TryParse<int>(Segment, Param)) { return false; }
			}

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, BasePrice)) { return false; }

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			IconPath = Segment;

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			SpritePath1 = Segment;

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			SpritePath2 = Segment;
		}
		catch (...)
		{
			return false;
		}

		return true;
	}

protected:
	FWeaponInfo() = default;
	~FWeaponInfo() override = default;
};
