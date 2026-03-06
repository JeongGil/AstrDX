#pragma once
#include <Table/TableInfoBase.h>

struct FCharacterVisualInfo : FTableInfoBase
{
	std::string Icon;
	std::string Eye;
	std::string Mouth;
	std::array<std::string, 5> Decos;

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

			if (!std::getline(Stream, Icon, Delim)) { assert(false); return false; }

			if (!std::getline(Stream, Eye, Delim)) { assert(false); return false; }

			if (!std::getline(Stream, Mouth, Delim)) { assert(false); return false; }

			for (auto& Deco : Decos)
			{
				if (!std::getline(Stream, Deco, Delim))
				{
					Deco = "";
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

	FCharacterVisualInfo() = default;
	~FCharacterVisualInfo() = default;
};
