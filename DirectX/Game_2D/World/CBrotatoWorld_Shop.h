#pragma once
#include <World/CWorld.h>

class CShopWidget;

class CBrotatoWorld_Shop :
    public CWorld
{
public:
	CBrotatoWorld_Shop() = default;
	~CBrotatoWorld_Shop() override = default;

private:
	std::weak_ptr<CShopWidget> ShopWidget;

public:
	bool Init() override;

	[[nodiscard]] std::weak_ptr<CShopWidget> GetShopWidget() const
	{
		return ShopWidget;
	}
};

