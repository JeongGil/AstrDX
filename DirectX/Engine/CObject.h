#pragma once

#include "EngineInfo.h"

class CObject : public std::enable_shared_from_this<CObject>
{
public:
	CObject() = default;
	CObject(const CObject& other) = default;
	CObject(CObject&& other) noexcept = default;
	CObject& operator=(const CObject& other) = default;
	CObject& operator=(CObject&& other) noexcept = default;
	virtual ~CObject() = 0;
};

