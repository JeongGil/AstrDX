#pragma once
#include <Table/TableInfoBase.h>

struct FMiscInfo : FTableInfoBase
{
	constexpr static size_t MATERIAL_TEX_COUNT = 11;

	std::string PotatoBodyTexPath;
	std::string PotatoLegTexPath;

	int WeaponMoveSpeed;

	std::string FruitTexPath;
	std::string ItemBoxTexPath;
	std::string LegendaryItemBoxTexPath;
	std::vector<std::string> MaterialTexPaths;

	float BasePickupRange;
	float PickupMoveSpeed;

	bool Load(std::stringstream& Stream) override
	{
		try
		{
			std::string Segment;

			//int IntVal;
			//uint8_t Uint8Val;

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			ID = TableID(1);

			if (!std::getline(Stream, PotatoBodyTexPath, Delim)) { assert(false); return false; }
			if (!std::getline(Stream, PotatoLegTexPath, Delim)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, WeaponMoveSpeed)) { assert(false); return false; }

			if (!std::getline(Stream, FruitTexPath, Delim)) { assert(false); return false; }
			if (!std::getline(Stream, ItemBoxTexPath, Delim)) { assert(false); return false; }
			if (!std::getline(Stream, LegendaryItemBoxTexPath, Delim)) { assert(false); return false; }

			for (size_t i = 0; i < MATERIAL_TEX_COUNT; i++)
			{
				if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
				if (!Segment.empty())
				{
					MaterialTexPaths.push_back(Segment);
				}
			}

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<float>(Segment, BasePickupRange)) { assert(false); return false; }
			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<float>(Segment, PickupMoveSpeed)) { assert(false); return false; }
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
