#pragma once
#ifndef _AUDIOMANAGER_H
#define _AUDIOMANAGER_H
#include "AssetManager.h"

class AudioManager {
	
private:

	static AudioManager* sInstance;

public:
	static AudioManager* Instance();
	static void Release();

	void playMusic(std::string filename, int loops = -1);
	void pauseMusic();
	void resumeMusic();

	void playSFX(std::string filename, int loops = 0, int channel = 0);

	AudioManager();
	~AudioManager();

};

#endif