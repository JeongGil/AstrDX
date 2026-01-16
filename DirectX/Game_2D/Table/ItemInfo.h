#pragma once
#include <Table/TableInfoBase.h>

struct FItemInfo : FTableInfoBase
{
	std::string Name;

	bool Load(std::stringstream& Stream) override
	{
		try
		{
			std::string Segment;

			int IntVal;
			//uint8_t Uint8Val;

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, IntVal)) { return false; }
			ID = TableID(IntVal);

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			Name = Segment;
		}
		catch (...)
		{
			return false;
		}

		return true;
	}

	FItemInfo() = default;
	~FItemInfo() = default;
};
