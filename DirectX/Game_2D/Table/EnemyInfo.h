#pragma once
#include <Table/TableInfoBase.h>

#include "../Defines.h"

struct FEnemyInfo : FTableInfoBase
{
	std::string Name;

	std::string IconPath;
	std::string SpritePath;

	EEnemyType Type;

	EEnemyBehavior::Type Behavior;

	float HP;
	float HpIncrease;

	int MinSpeed;
	int MaxSpeed;

	float Damage;
	float DamageIncrease;

	float KnockbackResist;

	int Material;
	int ConsumableDropPercent;
	int CrateDropPercent;

	std::array<int, MAX_DANGER + 1> AppearWaves;

	bool Load(std::stringstream& Stream) override
	{
		try
		{
			std::string Segment;

			int IntVal;
			uint8_t UInt8Val;
			uint32_t UInt32Val;

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, IntVal)) { assert(false); return false; }
			ID = TableID(IntVal);

			if (!std::getline(Stream, Name, Delim)) { assert(false); return false; }

			if (!std::getline(Stream, IconPath, Delim)) { assert(false); return false; }
			if (!std::getline(Stream, SpritePath, Delim)) { assert(false); return false; }

			if (!TryParse<uint8_t>(Segment, UInt8Val)) { assert(false); return false; }
			Type = static_cast<EEnemyType>(UInt8Val);

			if (!TryParse<uint32_t>(Segment, UInt32Val)) { assert(false); return false; }
			Behavior = static_cast<EEnemyBehavior::Type>(UInt32Val);

			if (!TryParse<float>(Segment, HP)) { assert(false); return false; }
			if (!TryParse<float>(Segment, HpIncrease)) { assert(false); return false; }

			if (!TryParse<int>(Segment, MinSpeed)) { assert(false); return false; }
			if (!TryParse<int>(Segment, MaxSpeed)) { assert(false); return false; }

			if (!TryParse<float>(Segment, Damage)) { assert(false); return false; }
			if (!TryParse<float>(Segment, DamageIncrease)) { assert(false); return false; }

			if (!TryParse<float>(Segment, KnockbackResist)) { assert(false); return false; }

			if (!TryParse<int>(Segment, Material)) { assert(false); return false; }
			if (!TryParse<int>(Segment, ConsumableDropPercent)) { assert(false); return false; }
			if (!TryParse<int>(Segment, CrateDropPercent)) { assert(false); return false; }

			for (size_t i = 0; i < MAX_DANGER; i++)
			{
				int& AppearWave = AppearWaves[i];
				if (!TryParse<int>(Segment, AppearWave)) { assert(false); return false; }

				if (AppearWave == 0)
				{
					if (i == 0)
					{
						AppearWave = 1;
					}
					else
					{
						AppearWave = AppearWaves[i - 1];
					}
				}
			}
		}
		catch (...)
		{
			assert(false);
			return false;
		}

		return true;
	}

	FEnemyInfo() = default;
	~FEnemyInfo() = default;
};
