#pragma once
#include <Table/TableInfoBase.h>

struct FCharacterVisualInfo : FTableInfoBase
{
	std::string Eye;
	std::string Mouth;
	std::array<std::string, 5> Decos;

	bool Load(std::stringstream& Stream) override
	{
		try
		{
			std::string Segment;

			int IntVal;
			uint8_t Uint8Val;

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			if (!TryParse<int>(Segment, IntVal)) { return false; }
			ID = TableID(IntVal);

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			Eye = Segment;

			if (!std::getline(Stream, Segment, Delim)) { return false; }
			Mouth = Segment;

			for (auto& Deco : Decos)
			{
				if (!std::getline(Stream, Segment, Delim)) { return false; }
				Deco = Segment;
			}
		}
		catch (...)
		{
			return false;
		}

		return true;
	}

	FCharacterVisualInfo() = default;
	~FCharacterVisualInfo() override = default;
};
