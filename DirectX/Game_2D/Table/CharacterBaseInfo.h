#pragma once
#include <EngineInfo.h>
#include <Table/TableInfoBase.h>

#include "../Defines.h"

struct FCharacterBaseInfo
	: FTableInfoBase
{
	std::string Name;

	std::string IconPath;
	std::string SpritePath;

	FVector2 ColliderSize;

	bool bIsMeleeWeapon;
	EWeaponType::Type WeaponType;

	int Tier;

	int BaseDamage;
	FDamageScale DamageScale1;
	FDamageScale DamageScale2;
	FDamageScale DamageScale3;

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

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, IntVal)) { assert(false); return false; }
			ID = TableID(IntVal);

			if (!std::getline(Stream, Name, Delim)) { assert(false); return false; }

			if (!std::getline(Stream, IconPath, Delim)) { assert(false); return false; }

			if (!std::getline(Stream, SpritePath, Delim)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<float>(Segment, ColliderSize.x)) { assert(false); return false; }
			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<float>(Segment, ColliderSize.y)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, IntVal)) { assert(false); return false; }
			bIsMeleeWeapon = IntVal == 1;

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<uint32_t>(Segment, Uint32Val)) { assert(false); return false; }
			WeaponType = static_cast<EWeaponType::Type>(Uint32Val);

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, Tier)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, BaseDamage)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<uint8_t>(Segment, Uint8Val)) { assert(false); return false; }
			DamageScale1.StatType = static_cast<EStat::Type>(Uint8Val);

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, DamageScale1.ScalePercent)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<uint8_t>(Segment, Uint8Val)) { assert(false); return false; }
			DamageScale2.StatType = static_cast<EStat::Type>(Uint8Val);

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, DamageScale2.ScalePercent)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<uint8_t>(Segment, Uint8Val)) { assert(false); return false; }
			DamageScale3.StatType = static_cast<EStat::Type>(Uint8Val);

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, DamageScale3.ScalePercent)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, CooldownMS)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, CritChancePercent)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, CritDamagePercent)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, Range)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, Knockback)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, LifeStealPercent)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<uint8_t>(Segment, Uint8Val)) { assert(false); return false; }
			SpecialEffect.Type = static_cast<ESpecialEffect::Type>(Uint8Val);

			for (auto& Param : SpecialEffect.Params)
			{
				if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
				if (!TryParse<int>(Segment, Param)) { assert(false); return false; }
			}

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, BasePrice)) { assert(false); return false; }
		}
		catch (...)
		{
			assert(false);
			return false;
		}

		return true;
	}

	FCharacterBaseInfo() = default;
	~FCharacterBaseInfo() = default;
};
