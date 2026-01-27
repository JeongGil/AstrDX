#include "CSoundManager.h"

#include <ranges>

CSoundManager::CSoundManager()
{
}

CSoundManager::~CSoundManager()
{
	Sounds.clear();

	for (const auto& Group : ChannelGroups | std::views::values)
	{
		Group->release();
	}
	ChannelGroups.clear();

	if (System)
	{
		System->release();
		System->close();
	}
	System = nullptr;
}

bool CSoundManager::Init()
{
	FMOD_RESULT Result = FMOD::System_Create(&System);
	if (Result != FMOD_OK)
	{
		return false;
	}

	Result = System->init(256, FMOD_INIT_NORMAL, nullptr);
	if (Result != FMOD_OK)
	{
		return false;
	}

	Result = System->getMasterChannelGroup(&MasterGroup);
	if (Result != FMOD_OK)
	{
		return false;
	}

	ChannelGroups.emplace("Master", MasterGroup);

	CreateChannelGroup("BGM");
	CreateChannelGroup("Effect");
	CreateChannelGroup("UI");

	return true;
}

void CSoundManager::Update(const float DeltaTime)
{
}

bool CSoundManager::CreateChannelGroup(const std::string& Key)
{
	FMOD::ChannelGroup* Group = FindChannelGroup(Key);
	if (Group)
	{
		return true;
	}

	FMOD_RESULT	result = System->createChannelGroup(Key.c_str(), &Group);
	if (result != FMOD_OK)
	{
		return false;
	}

	MasterGroup->addGroup(Group, false);

	ChannelGroups.emplace(Key, Group);

	return true;
}

bool CSoundManager::LoadSound(const std::string& Key, const std::string& GroupKey, bool bLoop, const char* FileName,
	const std::string& PathName)
{
	if (!FindSound(Key).expired())
	{
		return true;
	}

	FMOD::ChannelGroup* Group = FindChannelGroup(GroupKey);
	if (!Group)
	{
		Group = MasterGroup;
	}

	std::shared_ptr<CSound>	Sound;
	Sound.reset(new CSound);

	if (!Sound->LoadSound(System, Group, bLoop, FileName, PathName))
	{
		return false;
	}

	Sounds.emplace(Key, Sound);

	return true;
}

std::weak_ptr<CSound> CSoundManager::FindSound(const std::string& Key)
{
	auto It = Sounds.find(Key);

	if (It == Sounds.end())
	{
		return {};
	}

	return It->second;
}

void CSoundManager::Play(const std::string& Key)
{
	auto It = Sounds.find(Key);
	if (It == Sounds.end())
	{
		return;
	}

	It->second->Play();
}

void CSoundManager::Stop(const std::string& Key)
{
	auto It = Sounds.find(Key);
	if (It == Sounds.end())
	{
		return;
	}

	It->second->Stop();
}

void CSoundManager::Pause(const std::string& Key)
{
	auto It = Sounds.find(Key);
	if (It == Sounds.end())
	{
		return;
	}

	It->second->Pause();
}

void CSoundManager::Resume(const std::string& Key)
{
	auto It = Sounds.find(Key);
	if (It == Sounds.end())
	{
		return;
	}

	It->second->Resume();
}

void CSoundManager::SetMasterVolume(int Volume)
{
	MasterGroup->setVolume(std::clamp(Volume, 0, 100) / 100.f);
}

void CSoundManager::SetGroupVolume(const std::string& GroupKey, int Volume)
{
	auto Group = FindChannelGroup(GroupKey);
	if (!Group)
	{
		return;
	}

	Group->setVolume(std::clamp(Volume, 0, 100) / 100.f);
}

FMOD::ChannelGroup* CSoundManager::FindChannelGroup(const std::string& Key)
{
	auto It = ChannelGroups.find(Key);
	if (It == ChannelGroups.end())
	{
		return {};
	}

	return It->second;
}

void CSoundManager::ReleaseAsset(const std::string& Key)
{
	auto It = Sounds.find(Key);
	if (It != Sounds.end())
	{
		if (It->second.use_count() == 1)
		{
			Sounds.erase(It);
		}
	}
}
