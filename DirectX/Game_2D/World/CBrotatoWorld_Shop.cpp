#include "CBrotatoWorld_Shop.h"

#include "../UI/CShopWidget.h"

bool CBrotatoWorld_Shop::Init()
{
    if (!CWorld::Init())
    {
        return false;
    }

    ShopWidget = UIManager->CreateWidget<CShopWidget>("Shop");

    return true;
}
