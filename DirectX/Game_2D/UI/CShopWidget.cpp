#include "CShopWidget.h"

#include <CDevice.h>
#include <UI/CButton.h>
#include <UI/CImage.h>
#include <UI/CTextBlock.h>
#include <World/CWorldManager.h>

#include "CGoodsWidget.h"
#include "CItemInvenWidget.h"
#include "CStatWidget.h"
#include "CWeaponInvenWidget.h"
#include "../Strings.h"
#include "../Inventory/CCharacterData.h"
#include "../Inventory/CShop.h"
#include "../World/CLoadingWorld.h"

bool CShopWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	auto RS = CDevice::GetInst()->GetResolution();
	float Ratio = CDevice::GetInst()->GetRatioFHD();

	Background = CreateWidget<CImage>("BG");
	if (auto BG = Background.lock())
	{
		// CResultWidget::Init에서 이미 텍스쳐 로드.
		BG->SetTexture("ShopBG");

		BG->SetSize(RS.Width, RS.Height);
	}

	Goods = CreateWidget<CGoodsWidget>("Goods", 1);

	Stats = CreateWidget<CStatWidget>("Stats", 1);
	if (auto Widget = Stats.lock())
	{
		auto Pos = FVector(1483, 27, 0) * Ratio;
		Widget->SetPos(Pos);
	}

	Items = CreateWidget<CItemInvenWidget>("Items", 1);
	if (auto Widget = Items.lock())
	{
		// CGoodsWidget 하단 좌측, PlayButton 하단(1080-36=1044)에서 위젯 높이(96*2+4=196) 올림
		auto Pos = FVector(26, 848, 0) * Ratio;
		Widget->SetPos(Pos);
	}

	Weapons = CreateWidget<CWeaponInvenWidget>("Weapons", 1);
	if (auto Widget = Weapons.lock())
	{
		// CGoodsWidget 하단 우측(1460), 무기위젯 너비(96*3+4*2=296) 역산, PlayButton 하단 맞춤
		auto Pos = FVector(1164, 848, 0) * Ratio;
		Widget->SetPos(Pos);
	}


	// Title: 좌측 상단
	Title = CreateWidget<CTextBlock>("Title", 2);
	if (auto TitleText = Title.lock())
	{
		FVector2 TitlePos = FVector2(36, 36) * Ratio;
		TitleText->SetPos(TitlePos);
		TitleText->SetSize(500.f * Ratio, 80.f * Ratio);

		TitleText->SetText(TEXT("상점 (웨이브 "));
		TitleText->AddText(CCharacterData::GetInst().GetStageLevel());
		TitleText->AddText(TEXT(")"));

		TitleText->SetAlignH(ETextAlignH::Left);
		TitleText->SetAlignV(ETextAlignV::Top);

		auto FontSize = 36 * Ratio;
		TitleText->SetFontSize(FontSize);
		
		TitleText->SetTextColor(FColor::White);
	}

	// PlayButton: 우측 하단
	FVector2 ButtonSize = FVector2(400.f, 90.f) * Ratio;
	PlayButton = CreateWidget<CButton>("PlayButton", 1);
	if (auto Button = PlayButton.lock())
	{
		FVector2 ButtonPos = FVector2(RS.Width - 36 * Ratio, RS.Height - 36 * Ratio);

		Button->SetPivot(1, 1);
		Button->SetPos(ButtonPos);
		Button->SetSize(ButtonSize);
		Button->SetTint(EButtonState::Normal, 0, 0, 0, 0.6f);
		Button->SetTint(EButtonState::Hovered, 0.3f, 0.3f, 0.3f, 0.75f);

		Button->SetEventCallback<CShopWidget>(EButtonEventState::Click, this, &CShopWidget::OnClickPlay);

		if (auto Text = CWidget::CreateStaticWidget<CTextBlock>("PlayText", World, 2))
		{
			Playtext = Text;
			Text->SetPos(-ButtonSize.x, -ButtonSize.y);
			Text->SetSize(ButtonSize);

			Text->SetText(TEXT("이동 (웨이브 "));
			Text->AddText(CCharacterData::GetInst().GetStageLevel() + 1);
			Text->AddText(TEXT(")"));

			Text->SetAlignH(ETextAlignH::Center);
			Text->SetAlignV(ETextAlignV::Middle);
			Text->SetFontSize(45 * Ratio);
			Text->SetTextColor(FColor::White);
			Button->SetChild(Text);
		}
	}

	MaterialIcon = CreateWidget<CImage>("MaterialIcon", 1);
	if (auto Image = MaterialIcon.lock())
	{
		Image->SetTexture("harvesting_icon", TEXT("items/materials/harvesting_icon.png"), Key::Path::Brotato);

		auto Size = FVector(54, 54, 0) * Ratio;
		Image->SetSize(Size);

		auto Pos = FVector(RS.Width * 0.5f, 36, 0) * Ratio;
		Image->SetPos(Pos);
	}

	MaterialCount = CreateWidget<CTextBlock>("MaterialCount", 1);
	if (auto Text = MaterialCount.lock())
	{
		auto Size = FVector(500, 80, 0) * Ratio;
		Text->SetSize(Size);

		if (auto Image = MaterialIcon.lock())
		{
			auto Pos = Image->GetPos();
			Pos.x += Image->GetSize().x;
			Pos.y += (Image->GetSize().y - Text->GetSize().y) * 0.5f;

			Text->SetPos(Pos);
		}

		Text->SetTextColor(FColor::White);
		Text->SetText(CCharacterData::GetInst().GetMaterialCount());

		auto FontSize = 36 * Ratio;
		Text->SetFontSize(FontSize);
	}

	RerollButton = CreateWidget<CButton>("RerollButton", 1);
	if (auto Button = RerollButton.lock())
	{
		const float BtnW = 280.f * Ratio;
		const float BtnH = 70.f  * Ratio;

		// 우측 끝을 CGoodsWidget 우측 끝(1460)에 맞추고, 상단을 CStatWidget 상단(27)에 맞춤
		const float GoodsRight = (26.f + 4.f * 351.f + 3.f * 10.f) * Ratio; // 1460 * Ratio
		const float StatTop    = 27.f * Ratio;

		Button->SetPos(GoodsRight - BtnW, StatTop);
		Button->SetSize(BtnW, BtnH);
		Button->SetTint(EButtonState::Normal,  0.f,  0.f,  0.f,  0.6f);
		Button->SetTint(EButtonState::Hovered, 0.3f, 0.3f, 0.3f, 0.75f);
		Button->SetEventCallback<CShopWidget>(EButtonEventState::Click, this, &CShopWidget::OnClickReroll);

		const float IconSize      = 48.f * Ratio; // 32 * 1.5
		const float PriceFontSize = 24.f * Ratio;

		// RerollPrice 텍스트: 아이콘 왼쪽 영역을 채움
		RerollPrice = CreateWidget<CTextBlock>("RerollPriceText", 2);
		if (auto Text = RerollPrice.lock())
		{
			const int Price         = CShop::GetInst().GetRerollPrice();
			const int MaterialCount = CCharacterData::GetInst().GetMaterialCount();

			Text->SetPos(0.f, 0.f);
			Text->SetSize(BtnW - IconSize, BtnH);
			Text->SetAlignH(ETextAlignH::Center);
			Text->SetAlignV(ETextAlignV::Middle);
			Text->SetFontSize(PriceFontSize);
			Text->SetText(TEXT("초기화 - "));
			Text->AddText(Price);
			Text->SetTextColor(Price > MaterialCount ? FColor(1.f, 0.f, 0.f, 1.f) : FColor::White);
			Button->SetChild(RerollPrice);
		}

		// RerollIcon: SetChild가 내부적으로 버튼 크기로 덮어쓰므로 SetChild 이후에 크기/위치 재설정
		RerollIcon = CreateWidget<CImage>("RerollIconImg", 3);
		if (auto Icon = RerollIcon.lock())
		{
			Icon->SetTexture("harvesting_icon", TEXT("items/materials/harvesting_icon.png"), Key::Path::Brotato);
			Button->SetChild(RerollIcon);
			// SetChild 이후에 크기·위치 재지정 — 버튼 우측에 세로 중앙 정렬
			Icon->SetSize(IconSize, IconSize);
			Icon->SetPos(BtnW - IconSize, (BtnH - IconSize) * 0.5f);
		}
	}
	return true;
}

void CShopWidget::OnClickPlay()
{
	auto& CharacterData = CCharacterData::GetInst();
	CharacterData.SetStageLevel(CharacterData.GetStageLevel() + 1);

	if (auto World = CWorldManager::GetInst()->CreateWorld<CLoadingWorld>(true).lock())
	{
		World->Load(EWorldType::Main);
	}
}

void CShopWidget::SetCurrentLevel(const int NewLevel)
{
	if (CurrentLevel == NewLevel)
	{
		return;
	}

	CurrentLevel = NewLevel;

	if (auto Text = Title.lock())
	{
		Text->SetText(TEXT("상점 (웨이브 "));
		Text->AddText(GetCurrentLevel());
		Text->AddText(TEXT(")"));
	}

	if (auto Text = Playtext.lock())
	{
		Text->SetText(TEXT("이동 (웨이브 "));
		Text->AddText(GetNextLevel());
		Text->AddText(TEXT(")"));
	}
}

void CShopWidget::RefreshGoods()
{
	auto GoodsWidget = Goods.lock();
	if (!GoodsWidget)
	{
		return;
	}

	const auto& Shop = CShop::GetInst();
	for (size_t i = 0; i < 4; i++)
	{
		const auto& Goods = Shop.GetGoodsInfo(i);
		GoodsWidget->SetGoods(Goods, i);
	}
}

void CShopWidget::RefreshInventory()
{
	if (auto Widget = Weapons.lock())
	{
		Widget->Refresh();
	}

	if (auto Widget = Items.lock())
	{
		Widget->Refresh();
	}

	if (auto Text = MaterialCount.lock())
	{
		Text->SetText(CCharacterData::GetInst().GetMaterialCount());
	}
}

void CShopWidget::OnClickReroll()
{
	if (!CShop::GetInst().Reroll())
	{
		return;
	}

	RefreshGoods();

	if (auto Text = RerollPrice.lock())
	{
		const int Price         = CShop::GetInst().GetRerollPrice();
		const int MaterialCount = CCharacterData::GetInst().GetMaterialCount();

		Text->SetText(TEXT("초기화 - "));
		Text->AddText(Price);
		Text->SetTextColor(Price > MaterialCount ? FColor(1.f, 0.f, 0.f, 1.f) : FColor::White);
	}

	if (auto Text = MaterialCount.lock())
	{
		Text->SetText(CCharacterData::GetInst().GetMaterialCount());
	}
}
