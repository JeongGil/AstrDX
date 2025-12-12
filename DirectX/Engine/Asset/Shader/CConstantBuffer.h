#pragma once

#include "../../CObject.h"
#include "CBufferContainer.h"

class CConstantBuffer :
    public CObject
{
	friend class CShaderManager;

public:
	bool Init(int Size, int Register, int ShaderBuffer);
	void Update(const void* Data) const;

private:
	ID3D11Buffer* Buffer = nullptr;
	int Size = 0;
	
	// Register number of constant buffer.
	int Register = 0;

	// Used to determine which shader uses this constant buffer.
	int ShaderBuffer = 0;

private:
	CConstantBuffer() = default;

public:
	~CConstantBuffer() override;
};

