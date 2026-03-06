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

			if (!std::getline(Stream, Segment, Delim)) { assert(false); return false; }
			if (!TryParse<int>(Segment, IntVal)) { assert(false); return false; }
			ID = TableID(IntVal);

			if (!std::getline(Stream, Kr, Delim)) { assert(false); return false; }

			if (!std::getline(Stream, En, Delim)) { assert(false); return false; }
		}
		catch (...)
		{
			assert(false); 
			return false;
		}

		return true;
	}

	FStringInfo() = default;
	~FStringInfo() = default;
};

