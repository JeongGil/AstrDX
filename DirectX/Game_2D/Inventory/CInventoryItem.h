#pragma once
class CInventoryItem abstract
{
public:
	int GetDamageDealt()const
	{
		return DamageDealt;
	}

	void SetDamageDealt(int Amount)
	{
		DamageDealt = Amount;
	}
	void AddDamageDealt(int Amount)
	{
		DamageDealt += Amount;
	}

protected:
	int DamageDealt = 0;

	bool bWeapon = false;

public:
	CInventoryItem() = default;
	virtual ~CInventoryItem() = default;
};

