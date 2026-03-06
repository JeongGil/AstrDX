#pragma once
#include <Table/TableInfoBase.h>

struct FMiscInfo : FTableInfoBase
{
	std::string PotatoBodyTexPath;
	std::string PotatoLegTexPath;
	int WeaponMoveSpeed;

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
