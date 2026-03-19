#pragma once
#include <EngineInfo.h>
#include <Table/TableInfoBase.h>

#include "../Defines.h"

struct FCharacterBaseInfo
	: FTableInfoBase
{
	std::string PotatoBodyTexPath;
	std::string PotatoLegTexPath;

	float BasePickupRange;

	int HitColliderSizeX;
	int HitColliderSizeY;

	float BaseHP;
	float BaseSpeed;

	bool Load(std::stringstream& Stream) override
	{
		try
		{
			std::string Segment;

			int IntVal;
			//uint8_t Uint8Val;
			//uint32_t Uint32Val;

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, IntVal)) { assert(false); return false; }
			ID = TableID(IntVal);

			if (!std::getline(Stream, PotatoBodyTexPath, Delim)) { assert(false); return false; }

			if (!std::getline(Stream, PotatoLegTexPath, Delim)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<float>(Segment, BasePickupRange)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, HitColliderSizeX)) { assert(false); return false; }
			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, HitColliderSizeY)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<float>(Segment, BaseHP)) { assert(false); return false; }

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<float>(Segment, BaseSpeed)) { assert(false); return false; }
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
