#pragma once
#include <array>
#include <cstdint>

constexpr size_t INVENTORY_MAX_WEAPON = 6;

namespace EWeaponType
{
	enum Type : uint32_t
	{
		Naval = 1 << 0,
		Heavy = 1 << 1,
		Blunt = 1 << 2,
		Primitive = 1 << 3,
		Blade = 1 << 4,
		Tool = 1 << 5,
		Medical = 1 << 6,
		Unarmed = 1 << 7,
	};
}

namespace EStat
{
	enum Type : uint8_t
	{
		None = 0,

#pragma region Primary stat
		Level,
		MaxHP,
		HpRegen,
		/// <summary>
		/// Unit: %
		/// </summary>
		LifeSteal,
		/// <summary>
		/// Unit: %
		/// </summary>
		Damage,
		MeleeDamage,
		RangedDamage,
		ElementalDamage,
		/// <summary>
		/// Unit: %
		/// </summary>
		AttackSpeed,
		/// <summary>
		/// Unit: %
		/// </summary>
		CritChance,
		Engineering,
		Range,
		Armor,
		/// <summary>
		/// Unit: %
		/// </summary>
		Dodge,
		/// <summary>
		/// Unit: %
		/// </summary>
		Speed,
		Luck,
		Harvesting,
#pragma endregion

#pragma region Secondary stat
		ConsumableHeal,
		/// <summary>
		/// Unit: %
		/// </summary>
		MaterialsHeal,
		/// <summary>
		/// Unit: %
		/// </summary>
		XpGain,
		/// <summary>
		/// Unit: %
		/// </summary>
		PickupRange,
		/// <summary>
		/// Unit: %
		/// </summary>
		ItemPrice,
		/// <summary>
		/// Unit: %
		/// </summary>
		ExplosionDamage,
		/// <summary>
		/// Unit: %
		/// </summary>
		ExplosionSize,
		Bounces,
		Piercing,
		/// <summary>
		/// Unit: %
		/// </summary>
		PiercingDamage,
		/// <summary>
		/// Unit: %
		/// </summary>
		DamageToBoss,
		/// <summary>
		/// Unit: %
		/// </summary>
		StructureAttackSpeed,
		StructureRange,
		/// <summary>
		/// Unit: %
		/// </summary>
		BurningSpeed,
		BurningSpread,
		Knockback,
		/// <summary>
		/// Unit: %
		/// </summary>
		DoubleMaterialChance,
		FreeReroll,
		Trees,
		/// <summary>
		/// Unit: %
		/// </summary>
		Enemies,
		/// <summary>
		/// Unit: %
		/// </summary>
		EnemySpeed,
#pragma endregion
	};
}
namespace ESpecialEffect
{
	enum Type : uint8_t
	{
		None = 0,

		Consumable_HpRegen,

		EndOfWave_Armor,
		EndOfWave_XpGain,

		PickupMaterial_DamageEnemy,
		PickupMaterial_HealChance,

		LevelUp_HpRegen,

		Crate_Material,

		Pierce_Count,

		Stand_Armor,
		Stand_Dodge,
		Stand_HpRegen,

		Enemy_SpawnPercent,
		Enemy_Hp,
		Enemy_Damage,

		NextWave_XpGain,
		NextWave_EnemyHP,

		TakeDamagePerSec,

		DamagePerLifeSteal,
		DamagePerKnockback,

		ItemPrice,

		HarvestingIncrement,

		Adrenaline_Chance,
		Adrenaline_HealAmount,
		AlienEyes_ProjectileCount,
		AlienEyes_FireDelay,
		AlienEyes_BaseDamage,
		AlienEyes_MaxHpScale,
		Anvil_Armor,
		Axolotl,
		BabyElephant_Chance,
		BabyElephant_BaseDamage,
		BabyElephant_LuckScale,
		BabyGrecko_Chance,
		BabyWithABeard_BaseDamage,
		BabyWithABeard_RangeDamageScale,
		Bait_Min,
		Bait_Max,
		BallAndChain,
		Barnacle,
		BoneDice_DamageChance,
		BonDice_Damage,
		BonDice_MaxHpChance,
		BonDice_MaxHP,
		Cauldron_Damage,
		Cauldron_Duration,
		CandyBag_StatAmount,
		CandyBag_EliteChance,
		CommunitySupport_AttackSpeed,
		Crystal_AttackSpeed,

		// TODO: Start with Dangerous Bunny.
	};
}

struct FDamageScale
{
	EStat::Type DamageType = EStat::None;
	int ScalePercent = 0;
};

struct FSpecialEffect
{
	ESpecialEffect::Type Type;
	std::array<int, 10> Params;
};

struct FStat
{
	EStat::Type Type;
	int InnerValue;
	int WeightPercent;

	int GetWeightedValue() const
	{
		return InnerValue * WeightPercent * 0.01f;
	}
};
