#pragma once
#include "../CObject.h"

#include "../Utils.h"

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

	[[nodiscard]] size_t GetID() const
	{
		return ID;
	}

	[[nodiscard]] const std::string& GetKey() const
	{
		return Key;
	}

	void SetKey(const std::string& Key)
	{
		this->Key = Key;
		ID = CIDMaker::GetID(Key);
	}

protected:
	std::string Key;
	size_t ID;
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

