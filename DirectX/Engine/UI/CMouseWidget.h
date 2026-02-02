#pragma once
#include "CImage.h"

class CWorldUIManager;
class CWidgetContainer;
class CRenderManager;

class CMouseWidget :
    public CImage
{
	friend CWorldUIManager;
	friend CWidgetContainer;
	friend CWidget;
	friend CRenderManager;

protected:
	CMouseWidget();
	CMouseWidget(const CMouseWidget& other);

public:
	~CMouseWidget() override;

	bool SetTexture(const std::string& Key) override;
	bool SetTexture(const std::string& Key, const TCHAR* FileName, const std::string& PathName) override;
	virtual bool SetTextures(const std::string& Key, const std::vector<const TCHAR*>& FileNames, const std::string& PathName = "Texture");
	void AddBrushFrame(const FVector2& Start, const FVector2& Size, int Count);
	void AddBrushFrame(float StartX, float StartY, float SizeX, float SizeY, int Count);

	bool Init() override;
	void Update(const float DeltaTime) override;
	void Render() override;
	CMouseWidget* Clone() const override;
};

