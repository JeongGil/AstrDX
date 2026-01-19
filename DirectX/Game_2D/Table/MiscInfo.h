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

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			//if (!TryParse<int>(Segment, IntVal)) { return false; }
			//ID = TableID(IntVal);
			ID = TableID(1);

			if (!std::getline(Stream, PotatoBodyTexPath, Delim)) { return false; }

			if (!std::getline(Stream, PotatoLegTexPath, Delim)) { return false; }

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, WeaponMoveSpeed)) { return false; }
		}
		catch (...)
		{
			return false;
		}

		return true;
	}

	FMiscInfo() = default;
	~FMiscInfo() = default;
};
