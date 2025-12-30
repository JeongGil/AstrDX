#pragma once
#include "../CObject.h"

class CAsset :
	public CObject
{
	friend class CAssetManager;
	friend class CWorldAssetManager;

public:
	[[nodiscard]] EAssetType GetAssetType() const { return AssetType; }
	[[nodiscard]] const std::string& GetName() const { return Name; }
	void SetName(const std::string& Name) { this->Name = Name; }

protected:
	std::string Name;
	EAssetType AssetType = EAssetType::None;

protected:
	CAsset() = default;

	CAsset(const CAsset& other)
		: CObject(other),
		  Name(other.Name),
		  AssetType(other.AssetType)
	{
	}

	CAsset(CAsset&& other) noexcept
		: CObject(std::move(other)),
		  Name(std::move(other.Name)),
		  AssetType(other.AssetType)
	{
	}

	CAsset& operator=(const CAsset& other)
	{
		if (this == &other)
			return *this;
		CObject::operator =(other);
		Name = other.Name;
		AssetType = other.AssetType;
		return *this;
	}

	CAsset& operator=(CAsset&& other) noexcept
	{
		if (this == &other)
			return *this;
		CObject::operator =(std::move(other));
		Name = std::move(other.Name);
		AssetType = other.AssetType;
		return *this;
	}

public:
	virtual ~CAsset() = 0;
};

