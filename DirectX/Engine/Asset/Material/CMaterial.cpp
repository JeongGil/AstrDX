#include "CMaterial.h"

#include "../Shader/CCBufferMaterial.h"

void CMaterial::SetBaseColor(float r, float g, float b, float a)
{
	BaseColor = FColor(r, g, b, a);
	MaterialCBuffer->SetBaseColor(BaseColor);
}

void CMaterial::SetBaseColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	BaseColor = FColor(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
	MaterialCBuffer->SetBaseColor(BaseColor);
}

void CMaterial::SetBaseColor(const FColor& Color)
{
	BaseColor = Color;
	MaterialCBuffer->SetBaseColor(BaseColor);
}

void CMaterial::SetOpacity(float Opacity)
{
	this->Opacity = Opacity;
	MaterialCBuffer->SetOpacity(Opacity);
}

bool CMaterial::Init()
{
	MaterialCBuffer.reset(new CCBufferMaterial);
	MaterialCBuffer->Init();
	MaterialCBuffer->SetBaseColor(BaseColor);
	MaterialCBuffer->SetOpacity(Opacity);

	return true;
}

void CMaterial::UpdateConstantBuffer()
{
	MaterialCBuffer->UpdateBuffer();
}

CMaterial* CMaterial::Clone() const
{
	return new CMaterial(*this);
}

CMaterial::CMaterial(const CMaterial& other) : CObject(other),
BaseColor(other.BaseColor),
Opacity(other.Opacity)
//MaterialCBuffer(other.MaterialCBuffer)
{
	MaterialCBuffer.reset(new CCBufferMaterial);
	MaterialCBuffer->Init();
	MaterialCBuffer->SetBaseColor(BaseColor);
	MaterialCBuffer->SetOpacity(Opacity);
}

CMaterial::CMaterial(CMaterial&& other) noexcept : CObject(std::move(other)),
BaseColor(std::move(other.BaseColor)),
Opacity(other.Opacity),
MaterialCBuffer(std::move(other.MaterialCBuffer))
{
}

CMaterial& CMaterial::operator=(const CMaterial& other)
{
	if (this == &other)
		return *this;
	CObject::operator =(other);
	BaseColor = other.BaseColor;
	Opacity = other.Opacity;
	//MaterialCBuffer = other.MaterialCBuffer;
	MaterialCBuffer.reset(new CCBufferMaterial);
	MaterialCBuffer->Init();
	MaterialCBuffer->SetBaseColor(BaseColor);
	MaterialCBuffer->SetOpacity(Opacity);
	return *this;
}

CMaterial& CMaterial::operator=(CMaterial&& other) noexcept
{
	if (this == &other)
		return *this;
	CObject::operator =(std::move(other));
	BaseColor = std::move(other.BaseColor);
	Opacity = other.Opacity;
	MaterialCBuffer = std::move(other.MaterialCBuffer);
	return *this;
}
