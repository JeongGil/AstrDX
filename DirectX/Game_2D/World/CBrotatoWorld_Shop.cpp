#include "CBrotatoWorld_Shop.h"

#include "../Inventory/CShop.h"
#include "../UI/CShopWidget.h"

bool CBrotatoWorld_Shop::Init()
{
    if (!CWorld::Init())
    {
        return false;
    }

    ShopWidget = UIManager->CreateWidget<CShopWidget>("Shop");

    CShop::GetInst().GenerateGoods();

    if (auto Widget = ShopWidget.lock())
    {
        Widget->RefreshGoods();
    }

    return true;
}
