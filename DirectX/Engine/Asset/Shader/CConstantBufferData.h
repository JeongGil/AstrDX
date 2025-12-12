#pragma once

#include "../../EngineInfo.h"
#include "CBufferContainer.h"

class CConstantBuffer;

class CConstantBufferData abstract
{
public:
	virtual bool Init() = 0;
	virtual void UpdateBuffer() = 0;
	virtual CConstantBufferData* Clone() = 0;

protected:
	void SetConstantBuffer(const std::string& Key);

protected:
	std::weak_ptr<CConstantBuffer> Buffer;

public:
	CConstantBufferData() = default;
	virtual ~CConstantBufferData() = default;

	CConstantBufferData(const CConstantBufferData& other)
		: Buffer(other.Buffer)
	{
	}

	CConstantBufferData(CConstantBufferData&& other) noexcept
		: Buffer(std::move(other.Buffer))
	{
		other.Buffer.reset();
	}

	CConstantBufferData& operator=(const CConstantBufferData& other)
	{
		if (this == &other)
			return *this;
		Buffer = other.Buffer;
		return *this;
	}

	CConstantBufferData& operator=(CConstantBufferData&& other) noexcept
	{
		if (this == &other)
			return *this;
		Buffer = std::move(other.Buffer);
		other.Buffer.reset();
		return *this;
	}
};

