#include "CMouseWidget.h"

#include "../Asset/CAssetManager.h"
#include "../Asset/Texture/CTextureManager.h"
#include "../World/CWorldManager.h"

CMouseWidget::CMouseWidget()
{
}

CMouseWidget::CMouseWidget(const CMouseWidget& other) :
	CImage(other)
{
}

CMouseWidget::~CMouseWidget()
{
}

bool CMouseWidget::SetTexture(const std::string& Key)
{
	auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();
	auto Texture = TexMgr->FindTexture(Key);

	Brush.Texture = Texture;

	return true;
}

bool CMouseWidget::SetTexture(const std::string& Key, const TCHAR* FileName, const std::string& PathName)
{
	auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();

	if (!TexMgr->LoadTexture(Key, FileName, PathName))
	{
		return false;
	}

	auto Texture = TexMgr->FindTexture(Key);

	Brush.Texture = Texture;

	return true;
}

bool CMouseWidget::SetTextures(const std::string& Key, const std::vector<const TCHAR*>& FileNames,
	const std::string& PathName)
{
	auto TexMgr = CAssetManager::GetInst()->GetTextureManager().lock();

	if (!TexMgr->LoadTextures(Key, FileNames, PathName))
	{
		return false;
	}

	auto Texture = TexMgr->FindTexture(Key);

	Brush.Texture = Texture;

	return true;
}

void CMouseWidget::AddBrushFrame(const FVector2& Start, const FVector2& Size, int Count)
{
	for (int i = 0; i < Count; i++)
	{
		Brush.AnimationFrames.emplace_back(Start, Size);
	}

	Brush.FrameTime = Brush.PlayTime / Brush.AnimationFrames.size();
}

void CMouseWidget::AddBrushFrame(float StartX, float StartY, float SizeX, float SizeY, int Count)
{
	AddBrushFrame(FVector2(StartX, StartY), FVector2(SizeX, SizeY), Count);
}

bool CMouseWidget::Init()
{
	if (!CImage::Init())
	{
		return false;
	}

	return true;
}

void CMouseWidget::Update(const float DeltaTime)
{
	CImage::Update(DeltaTime);

	if (auto World = CWorldManager::GetInst()->GetWorld().lock())
	{
		auto Input = World->GetInput().lock();
		auto MousePos = Input->GetMousePos();
		SetPos(MousePos.x, MousePos.y);
	}
}

void CMouseWidget::Render()
{
	CImage::Render();
}

CMouseWidget* CMouseWidget::Clone() const
{
	return new CMouseWidget(*this);
}
