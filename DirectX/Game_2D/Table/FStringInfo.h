#pragma once
#include <Table\TableInfoBase.h>

struct FStringInfo : public FTableInfoBase
{
	std::string Kr;
	std::string En;

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

			if (!std::getline(Stream, Kr, Delim)) { return false; }

			if (!std::getline(Stream, En, Delim)) { return false; }
		}
		catch (...)
		{
			return false;
		}

		return true;
	}

	FStringInfo() = default;
	~FStringInfo() = default;
};

