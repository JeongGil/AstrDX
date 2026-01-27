#pragma once

#include "CSound.h"

class CAssetManager;

class CSoundManager
{
	friend CAssetManager;

private:
	CSoundManager();

public:
	~CSoundManager();

private:
	std::unordered_map<std::string, std::shared_ptr<CSound>> Sounds;
	std::unordered_map<std::string, FMOD::ChannelGroup*> ChannelGroups;

	FMOD::System* System = nullptr;
	FMOD::ChannelGroup* MasterGroup = nullptr;

public:
	bool Init();
	void Update();
	bool CreateChannelGroup(const std::string& Key);
	bool LoadSound(const std::string& Key, const std::string& GroupKey, bool bLoop,
		const char* FileName, const std::string& PathName = "Sound");
	std::weak_ptr<CSound> FindSound(const std::string& Key);

	void Play(const std::string& Key);
	void Stop(const std::string& Key);
	void Pause(const std::string& Key);
	void Resume(const std::string& Key);

	void SetMasterVolume(int Volume);
	void SetGroupVolume(const std::string& GroupKey, int Volume);

private:
	FMOD::ChannelGroup* FindChannelGroup(const std::string& Key);

public:
	void ReleaseAsset(const std::string& Key);
};

