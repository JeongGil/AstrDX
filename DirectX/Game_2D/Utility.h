#pragma once
#include <EngineInfo.h>
#include <Component/CMeshComponent.h>
#include <Component/CColliderBox2D.h>
#include <Asset/Material/CMaterial.h>
#include <Asset/Texture/CTexture.h>

inline std::string TCharToStdString(const TCHAR* tcharStr)
{
	if (!tcharStr) return "";

#ifdef UNICODE
	int size = WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, nullptr, 0, nullptr, nullptr);
	if (size <= 0) return "";

	std::string result(size - 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, &result[0], size, nullptr, nullptr);
	return result;
#else
	return std::string(tcharStr);
#endif
}

inline void SetMeshSizeFromTexture(
	const std::weak_ptr<FMaterialTextureInfo>& MatTexInfo,
	const std::weak_ptr<CMeshComponent>& Mesh)
{
	if (auto MatTexInfoPtr = MatTexInfo.lock())
	{
		if (auto Texture = MatTexInfoPtr->Texture.lock())
		{
			auto TexInfo = Texture->GetTexture();
			if (auto MeshPtr = Mesh.lock())
			{
				MeshPtr->SetWorldScale(TexInfo->Width, TexInfo->Height);
			}
		}
	}
}

inline const TCHAR* GetStatName(EStat::Type StatType)
{
	switch (StatType)
	{
	case EStat::Level:
		return TEXT("현재 레벨");
	case EStat::MaxHP:
		return TEXT("최대 HP");
	case EStat::HpRegen:
		return TEXT("HP 재생");
	case EStat::LifeSteal:
		return TEXT("% 생명 훔침");
	case EStat::Damage:
		return TEXT("% 대미지");		
	case EStat::MeleeDamage:
		return TEXT("근거리 대미지");
	case EStat::RangedDamage:
		return TEXT("원거리 대미지");
	case EStat::ElementalDamage:
		return TEXT("원소 대미지");
	case EStat::AttackSpeed:
		return TEXT("% 공격 속도");
	case EStat::CritChance:
		return TEXT("% 치명타율");
	case EStat::Engineering:
		return TEXT("엔지니어링");
	case EStat::Range:
		return TEXT("범위");
	case EStat::Armor:
		return TEXT("방어구");
	case EStat::Dodge:
		return TEXT("% 회피");
	case EStat::Speed:
		return TEXT("% 속도");
	case EStat::Luck:
		return TEXT("행운");
	case EStat::Harvesting:
		return TEXT("수확");
	case EStat::ConsumableHeal:
		return TEXT("소모품 치료");
	case EStat::MaterialsHeal:
		return TEXT("% 재료 치료");
	case EStat::XpGain:
		return TEXT("% XP 획득");
	case EStat::PickupRange:
		return TEXT("% 획득 범위");
	case EStat::ItemPrice:
		return TEXT("% 아이템 가격");
	case EStat::ExplosionDamage:
		return TEXT("% 폭발 대미지");
	case EStat::ExplosionSize:
		return TEXT("% 폭발 크기");
	case EStat::Bounces:
		return TEXT("튕김");
	case EStat::Piercing:
		return TEXT("관통");
	case EStat::PiercingDamage:
		return TEXT("% 관통 대미지");
	case EStat::DamageToBoss:
		return TEXT("% 보스에 대한 대미지");
	case EStat::StructureAttackSpeed:
		return TEXT("% 건축물 공격 속도");
	case EStat::StructureRange:
		return TEXT("구조 범위");
	case EStat::BurningSpeed:
		return TEXT("% 화상 속도");
	case EStat::BurningSpread:
		return TEXT("% 화상 속도");
	case EStat::Knockback:
		return TEXT("넉백");
	case EStat::DoubleMaterialChance:
		return TEXT("% 재료 두배 확률");
	case EStat::FreeReroll:
		return TEXT("무료 초기화");
	case EStat::Trees:
		return TEXT("나무");
	case EStat::Enemies:
		return TEXT("% 적");
	case EStat::EnemySpeed:
		return TEXT("% 적 속도");
	default:
		return TEXT("");
	}
}
