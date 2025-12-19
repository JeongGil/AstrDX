#pragma once
#include "../../CObject.h"

class CCBufferMaterial;

class CMaterial :
    public CObject
{
	friend class CMaterialManager;
	friend class CMesh;

public:
	void SetBaseColor(float r, float g, float b, float a);
	void SetBaseColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetBaseColor(const FColor& Color);
	void SetOpacity(float Opacity);

protected:
	FColor BaseColor = FColor::White;
	float Opacity = 1.f;

	std::shared_ptr<CCBufferMaterial> MaterialCBuffer;

public:
	bool Init();
	void UpdateConstantBuffer();
	CMaterial* Clone() const;

protected:
	CMaterial() = default;

	CMaterial(const CMaterial& other);

	CMaterial(CMaterial&& other) noexcept;

	CMaterial& operator=(const CMaterial& other);

	CMaterial& operator=(CMaterial&& other) noexcept;

public:
	~CMaterial() override = default;
};

