#include "AudioManager.h"
#include "Game.hpp"

AudioManager* AudioManager::sInstance = NULL;

AudioManager* AudioManager::Instance() {
	if (sInstance == NULL) {
		sInstance = new AudioManager();
	}

	return sInstance;
}

void AudioManager::Release() {

	delete sInstance;
	sInstance = NULL;
}

AudioManager::AudioManager(){

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		printf("Mixer Initialization Error: %s\n", Mix_GetError());
	}
}

AudioManager::~AudioManager() {
	Mix_Quit();
}

void AudioManager::playMusic(std::string filename, int loops) {
	Mix_PlayMusic(Game::assets->getMusic(filename), loops);
}

void AudioManager::pauseMusic() {
	if (Mix_PlayingMusic() != 0) {
		Mix_PauseMusic();
	}
}

void AudioManager::resumeMusic() {
	if (Mix_PausedMusic() != 0) {
		Mix_ResumeMusic();
	}
}

void AudioManager::playSFX(std::string filename, int loops, int channel) {
	Mix_PlayChannel(channel, Game::assets->getSFX(filename), loops);
}



