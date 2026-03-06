#pragma once
#include <Table/TableInfoBase.h>

struct FProjectileInfo : FTableInfoBase
{
	static constexpr size_t MAX_PROJECTILE_SPRITE_COUNT = 10;
	std::string Name;

	std::vector<std::string> SpritePaths;

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

			for (size_t i = 0; i < MAX_PROJECTILE_SPRITE_COUNT; i++)
			{
				if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
				if (Segment.empty())
				{
					break;
				}

				SpritePaths.push_back(Segment);
			}
		}
		catch (...)
		{
			assert(false);
			return false;
		}

		return true;
	}

	FProjectileInfo() = default;
	~FProjectileInfo() = default;
};
