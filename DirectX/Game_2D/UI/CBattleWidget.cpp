#include "CBattleWidget.h"

#include <CDevice.h>

#include "CLootStateWidget.h"
#include "CPCStateWidget.h"
#include "CStageStateWidget.h"

bool CBattleWidget::Init()
{
	if (!CWidgetContainer::Init())
	{
		return false;
	}

	const auto& Resolution = CDevice::GetInst()->GetResolution();

	PCStateWidget = CreateWidget<CPCStateWidget>("PCState");
	if (auto Widget = PCStateWidget.lock())
	{
		Widget->SetPos(0.f, 0.f);
	}

	StageStateWidget = CreateWidget<CStageStateWidget>("StageState");
	if (auto Widget = StageStateWidget.lock())
	{
		const float HalfWidth = Resolution.Width * 0.5f;
		Widget->SetPivot(0.5f, 0.f);
		Widget->SetPos(HalfWidth, 0.f);
	}

	LootStateWidget = CreateWidget<CLootStateWidget>("LootState");
	if (auto Widget = LootStateWidget.lock())
	{
		Widget->SetPivot(1.f, 0.f);
		Widget->SetPos(Resolution.Width, 0.f);
	}

	return true;
}

void CBattleWidget::SetPlayerCharacter(const std::weak_ptr<CPlayerCharacter>& PC)
{
	if (auto Widget = PCStateWidget.lock())
	{
		Widget->SetPlayerCharacter(PC);
	}
}
