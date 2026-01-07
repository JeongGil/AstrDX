#include "CCollisionInfoManager.h"

#include <ranges>

bool CCollisionInfoManager::Init()
{
	CreateChannel("Static");
	CreateChannel("Player");
	CreateChannel("Monster");

	CreateProfile("Static", "Static", true);
	CreateProfile("Player", "Player", true);
	CreateProfile("Monster", "Monster", true);

	return true;
}

void CCollisionInfoManager::CreateChannel(const std::string& Key)
{
	if (CreateChannelTarget == ECollisionChannel::End)
	{
		return;
	}

	if (Channels.contains(Key))
	{
		return;
	}

	auto Channel = new FCollisionChannel
	{
		.Name = Key,
		.Channel = CreateChannelTarget
	};

	if (Channels.try_emplace(Key, Channel).second)
	{
		CreateChannelTarget = static_cast<ECollisionChannel::Type>(CreateChannelTarget + 1);
	}
	else
	{
		delete Channel;
	}
}

bool CCollisionInfoManager::CreateProfile(const std::string& Key, const std::string& ChannelKey, bool bEnable,
	ECollisionInteraction::Type DefaultInteraction)
{
	if (Profiles.contains(Key))
	{
		return true;
	}

	auto Channel = FindChannel(ChannelKey);
	if (!Channel)
	{
		return false;
	}

	auto Profile = new FCollisionProfile
	{
		.Name = Key,
		.Channel = Channel,
		.bEnable = bEnable
	};

	for (auto& Interaction : Profile->Interaction)
	{
		Interaction = DefaultInteraction;
	}

	if (Profiles.try_emplace(Key, Profile).second)
	{
		return true;
	}
	else
	{
		delete Profile;
		return false;
	}
}

bool CCollisionInfoManager::CreateProfile(const std::string& Key, ECollisionChannel::Type Channel, bool bEnable,
	ECollisionInteraction::Type DefaultInteraction)
{
	if (Profiles.contains(Key))
	{
		return true;
	}

	FCollisionChannel* ChannelInst = nullptr;
	for (const auto& Chan : Channels | std::views::values)
	{
		if (Chan->Channel == Channel)
		{
			ChannelInst = Chan;
			break;
		}
	}

	if (!ChannelInst)
	{
		return false;
	}

	auto Profile = new FCollisionProfile
	{
		.Name = Key,
		.Channel = ChannelInst,
		.bEnable = bEnable
	};

	for (auto& Interaction : Profile->Interaction)
	{
		Interaction = DefaultInteraction;
	}

	if (Profiles.try_emplace(Key, Profile).second)
	{
		return true;
	}
	else
	{
		delete Profile;
		return false;
	}
}

bool CCollisionInfoManager::SetProfileInteraction(const std::string& Key, const std::string& ChannelKey,
	ECollisionInteraction::Type Interaction)
{
	auto Profile = FindProfile(Key);
	if (!Profile)
	{
		return false;
	}

	auto Channel = FindChannel(ChannelKey);
	if (!Channel)
	{
		return false;
	}

	Profile->Interaction[Channel->Channel] = Interaction;

	return true;
}

bool CCollisionInfoManager::SetProfileInteraction(const std::string& Key, ECollisionChannel::Type Channel,
	ECollisionInteraction::Type Interaction)
{
	auto Profile = FindProfile(Key);
	if (!Profile)
	{
		return false;
	}

	Profile->Interaction[Channel] = Interaction;

	return true;
}

bool CCollisionInfoManager::SetProfileEnable(const std::string& Key, bool bEnable)
{
	auto Profile = FindProfile(Key);
	if (!Profile)
	{
		return false;
	}

	Profile->bEnable = bEnable;
	return true;
}

FCollisionProfile* CCollisionInfoManager::FindProfile(const std::string& Key)
{
	auto It = Profiles.find(Key);
	return It == Profiles.end() ? nullptr : It->second;
}

FCollisionChannel* CCollisionInfoManager::FindChannel(const std::string& Key)
{
	auto It = Channels.find(Key);
	return It == Channels.end() ? nullptr : It->second;
}

CCollisionInfoManager::~CCollisionInfoManager()
{
	for (auto& Profile : Profiles | std::views::values)
	{
		SAFE_DELETE(Profile);
	}

	for (auto& Channel : Channels | std::views::values)
	{
		SAFE_DELETE(Channel);
	}
}
