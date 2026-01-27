#include "CSound.h"

#include "../CPathManager.h"

CSound::CSound()
{
}

CSound::~CSound()
{
	if (Sound)
	{
		Sound->release();
	}
}

bool CSound::LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group, bool bLoop, const char* FileName,
	const std::string& PathName)
{
	this->System = System;
	this->Group = Group;
	this->bLoop = bLoop;

	char FullPath[MAX_PATH] = {};

	if (const char* Path = CPathManager::FindPathMultibyte(PathName))
	{
		strcpy_s(FullPath, Path);
	}

	strcat_s(FullPath, FileName);

	FMOD_MODE Mode = FMOD_DEFAULT;

	if (bLoop)
	{
		Mode = FMOD_LOOP_NORMAL;
	}

	if (System->createSound(FullPath, Mode, nullptr, &Sound) != FMOD_OK)
	{
		return false;
	}

	return true;
}

void CSound::Play()
{
	System->playSound(Sound, Group, false, &Channel);

	++PlayCount;

	Channel->setCallback(CSound::SoundPlayEnd);
}

void CSound::Stop()
{
	if (Channel)
	{
		--PlayCount;

		bool IsPlaying = false;

		Channel->isPlaying(&IsPlaying);

		if (IsPlaying)
		{
			Channel->stop();

			if (PlayCount == 0)
			{
				Channel = nullptr;
			}
		}
		else
		{
			Channel = nullptr;
		}
	}
}

void CSound::Pause()
{
	if (Channel)
	{
		bool IsPlaying = false;

		Channel->isPlaying(&IsPlaying);

		if (IsPlaying)
		{
			Channel->setPaused(true);
		}
	}
}

void CSound::Resume()
{
	if (Channel)
	{
		bool IsPaused = false;

		Channel->getPaused(&IsPaused);

		if (IsPaused)
		{
			Channel->setPaused(false);
		}
	}
}

FMOD_RESULT CSound::SoundPlayEnd(FMOD_CHANNELCONTROL* ChannelControl, FMOD_CHANNELCONTROL_TYPE ControlType,
	FMOD_CHANNELCONTROL_CALLBACK_TYPE CallbackType, void* CommandData1, void* CommandData2)
{
	return FMOD_OK;
}
