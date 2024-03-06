#pragma once
#include "Singleton.h"

class SoundMgr : public Singleton<SoundMgr>
{
	friend Singleton<SoundMgr>;
private:
	SoundMgr();
	virtual ~SoundMgr();

	sf::Sound bgm[2];
	int frontBgmIndex = 0;
	int backBgmIndex = 1;

	bool isFading = false;
	float fadeSpeed = 0.5f;
	float fadeLimit = 0.05;

	std::list<sf::Sound*> playing;
	std::list<sf::Sound*> waiting;

	float sfxVolume = 100.f;
	float bgmVolume = 50.f;

public:
	void SetSfxVolume(float v) { sfxVolume = v; }
	void SetBgmVolume(float v) { bgmVolume = v; }

	void Init(int totalChannels = 64);
	void Release();
	void Update(float dt);

	void PlayBgm(std::string id, bool crossFade = true);
	void StopBgm();

	void PlaySfx(std::string id, bool loop = false);
	void PlaySfx(sf::SoundBuffer& buffer, bool loop = false);

	void StopAll();
};

#define SOUND_MGR (Singleton<SoundMgr>::Instance())

