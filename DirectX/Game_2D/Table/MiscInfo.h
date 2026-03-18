#pragma once
#include <Table/TableInfoBase.h>

struct FMiscInfo : FTableInfoBase
{
	constexpr static size_t MATERIAL_TEX_COUNT = 11;
	constexpr static size_t HIT_EFFECT_TEX_COUNT = 3;

	int WeaponMoveSpeed;

	std::string FruitTexPath;
	std::string ItemBoxTexPath;
	std::string LegendaryItemBoxTexPath;
	std::vector<std::string> MaterialTexPaths;

	float PickupMoveSpeed;

	int MapSizeX;
	int MapSizeY;

	std::string EnemySpawnMarkerTexPath;
	std::string TurretSpawnMarkerTexPath;

	float EnemySpawningTimeSec;

	std::vector<std::string> HitEffectTexPaths;

	bool Load(std::stringstream& Stream) override
	{
		try
		{
			std::string Segment;

			//int IntVal;
			//uint8_t Uint8Val;

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			ID = TableID(1);

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, WeaponMoveSpeed)) { assert(false); return false; }

			if (!std::getline(Stream, FruitTexPath, Delim)) { assert(false); return false; }
			if (!std::getline(Stream, ItemBoxTexPath, Delim)) { assert(false); return false; }
			if (!std::getline(Stream, LegendaryItemBoxTexPath, Delim)) { assert(false); return false; }

			MaterialTexPaths.reserve(MATERIAL_TEX_COUNT);
			for (size_t i = 0; i < MATERIAL_TEX_COUNT; i++)
			{
				if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
				if (!Segment.empty())
				{
					MaterialTexPaths.push_back(Segment);
				}
			}

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<float>(Segment, PickupMoveSpeed)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, MapSizeX)) { assert(false); return false; }
			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, MapSizeY)) { assert(false); return false; }

			if (!std::getline(Stream, EnemySpawnMarkerTexPath, Delim)) { assert(false); return false; }
			if (!std::getline(Stream, TurretSpawnMarkerTexPath, Delim)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<float>(Segment, EnemySpawningTimeSec)) { assert(false); return false; }

			HitEffectTexPaths.reserve(HIT_EFFECT_TEX_COUNT);
			for (size_t i = 0; i < HIT_EFFECT_TEX_COUNT; i++)
			{
				if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
				if (!Segment.empty())
				{
					HitEffectTexPaths.push_back(Segment);
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

	FMiscInfo() = default;
	~FMiscInfo() = default;
};
