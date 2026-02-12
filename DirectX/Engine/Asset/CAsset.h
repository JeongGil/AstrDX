#pragma once
#include "../CObject.h"

#include "../Utils.h"

class CAssetManager;
class CWorldAssetManager;

class CAsset :
	public CObject
{
	friend CAssetManager;
	friend CWorldAssetManager;

protected:
	CAsset() = default;
	CAsset(const CAsset& other) = default;
	CAsset(CAsset&& other) noexcept = default;

public:
	virtual ~CAsset() = 0;

protected:
	std::string Key;
	size_t ID;
	EAssetType AssetType{ EAssetType::None };
	bool bKeep{ false };

public:
	[[nodiscard]] bool GetKeep() const
	{
		return bKeep;
	}

	void SetKeep(const bool bKeep)
	{
		this->bKeep = bKeep;
	}

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
};

