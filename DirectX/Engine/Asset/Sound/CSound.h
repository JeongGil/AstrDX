#pragma once
#include "../CAsset.h"

#include "fmod.hpp"

#pragma comment(lib, "fmod_vc.lib")

class CSoundManager;

class CSound :
    public CAsset
{
	friend CSoundManager;

protected:
	CSound();

public:
	~CSound() override;

protected:
	FMOD::System* System = nullptr;
	FMOD::ChannelGroup* Group = nullptr;
	FMOD::Channel* Channel = nullptr;
	FMOD::Sound* Sound = nullptr;

	int PlayCount = 0;
	bool bLoop = false;

public:
	bool LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group, bool bLoop,
		const char* FileName, const std::string& PathName = "Sound");

	void Play();
	void Stop();
	void Pause();
	void Resume();

	static FMOD_RESULT SoundPlayEnd(
		FMOD_CHANNELCONTROL* ChannelControl,
		FMOD_CHANNELCONTROL_TYPE ControlType,
		FMOD_CHANNELCONTROL_CALLBACK_TYPE CallbackType,
		void* CommandData1, void* CommandData2);
};

