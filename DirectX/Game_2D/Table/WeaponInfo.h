#pragma once
#include <EngineInfo.h>
#include <Table/TableInfoBase.h>

#include "../Defines.h"

struct FWeaponInfo
	: FTableInfoBase
{
	std::string Name;
	
	std::string IconPath;
	std::string SpritePath;
	
	EWeaponType::Type Type;

	int Tier;

	int BaseDamage;
	FDamageScale DamageScale1;
	FDamageScale DamageScale2;
	
	int CooldownMS;
	
	int CritChancePercent;
	int CritDamagePercent;

	int Range;
	int Knockback;
	int LifeStealPercent;

	FSpecialEffect SpecialEffect;
	
	int BasePrice;

	bool Load(std::stringstream& Stream) override
	{
		try
		{
			std::string Segment;

			int IntVal;			
			uint8_t Uint8Val;
			uint32_t Uint32Val;

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, IntVal)) { return false; }
			ID = TableID(IntVal);

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			Name = Segment;

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			IconPath = Segment;

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			SpritePath = Segment;

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<uint32_t>(Segment, Uint32Val)) { return false; }
			Type = static_cast<EWeaponType::Type>(Uint32Val);

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, Tier)) { return false; }

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
			if (!TryParse<int>(Segment, CooldownMS)) { return false; }

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

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<uint8_t>(Segment, Uint8Val)) { return false; }
			SpecialEffect.Type = static_cast<ESpecialEffect::Type>(Uint8Val);

			for (auto& Param : SpecialEffect.Params)
			{
				if (!std::getline(Stream, Segment, Delim)) { return false; }
				if (!TryParse<int>(Segment, Param)) { return false; }
			}

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, BasePrice)) { return false; }
		}
		catch (...)
		{
			return false;
		}

		return true;
	}

	FWeaponInfo() = default;
	~FWeaponInfo() = default;
};
