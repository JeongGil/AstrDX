#pragma once
#include <sstream>
#include "../EngineInfo.h"

struct FTableInfoBase abstract
{
	inline const static char Delim = ',';

	TableID ID = TableID(-1);

	virtual bool Load(std::stringstream& Stream) = 0;

protected:
	template <typename T>
	bool TryParse(const std::string& Str, T& OutValue)
	{
		auto [Ptr, Errc] = std::from_chars(Str.data(), Str.data() + Str.size(), OutValue);
		return Errc == std::errc() && Ptr == Str.data() + Str.size();
	}
};
