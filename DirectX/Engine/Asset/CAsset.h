#pragma once
#include "../CObject.h"

class CAsset :
	public CObject
{
	friend class CAssetManager;
	friend class CWorldAssetManager;

public:
	[[nodiscard]] EAssetType GetAssetType() const
	{
		return AssetType;
	}

	[[nodiscard]] const std::string& GetKey() const
	{
		return Key;
	}
	void SetKey(const std::string& Key)
	{
		this->Key = Key;
	}

protected:
	std::string Key;
	EAssetType AssetType = EAssetType::None;

protected:
	CAsset() = default;
	CAsset(const CAsset& other) = default;
	CAsset(CAsset&& other) noexcept = default;
	CAsset& operator=(const CAsset& other) = default;
	CAsset& operator=(CAsset&& other) noexcept = default;

public:
	virtual ~CAsset() = 0;
};

