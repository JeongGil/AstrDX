#pragma once

#include "EngineInfo.h"

class CCollisionInfoManager
{
public:
	bool Init();

	void CreateChannel(const std::string& Key);
	
	bool CreateProfile(const std::string& Key, const std::string& ChannelKey, bool bEnable, ECollisionInteraction::Type DefaultInteraction = ECollisionInteraction::Collision);
	bool CreateProfile(const std::string& Key, ECollisionChannel::Type Channel, bool bEnable, ECollisionInteraction::Type DefaultInteraction = ECollisionInteraction::Collision);

	bool SetProfileInteraction(const std::string& Key, const std::string& ChannelKey, ECollisionInteraction::Type Interaction);
	bool SetProfileInteraction(const std::string& Key, ECollisionChannel::Type Channel, ECollisionInteraction::Type Interaction);

	bool SetProfileEnable(const std::string& Key, bool bEnable);

	FCollisionProfile* FindProfile(const std::string& Key);

private:
	FCollisionChannel* FindChannel(const std::string& Key);

private:
	std::unordered_map<std::string, FCollisionChannel*> Channels;
	ECollisionChannel::Type CreateChannelTarget = ECollisionChannel::Static;

	std::unordered_map<std::string, FCollisionProfile*> Profiles;

private:
	inline static CCollisionInfoManager* Inst = nullptr;

public:
	static CCollisionInfoManager* GetInst()
	{
		if (!Inst)
		{
			Inst = new CCollisionInfoManager;
		}

		return Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(Inst);
	}

private:
	CCollisionInfoManager() = default;
	~CCollisionInfoManager();
};

