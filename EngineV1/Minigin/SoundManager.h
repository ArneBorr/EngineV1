#pragma once
#include "Singleton.h"
#include <SDL_mixer.h>
#include "Music2D.h"
#include "Effect2D.h"
#include "ResourceManager.h"

struct MusicObject
{
	MusicObject(const std::string& name, const std::string& pth = " ") 
	{
		strcpy_s(id, name.c_str());
		strcpy_s(path, pth.c_str());
		pMusic = ResourceManager::GetInstance()->LoadMusic(path);
	}

	~MusicObject() { delete pMusic; pMusic = nullptr; }
	char id[40];
	char path[40];
	Music2D* pMusic{ nullptr };
};

struct EffectObject
{
	EffectObject(const std::string& name, const std::string& pth = " ") 
	{
		strcpy_s(id, name.c_str());
		strcpy_s(path, pth.c_str());
		pEffect = ResourceManager::GetInstance()->LoadSoundEffect(path);
	}
	~EffectObject() { delete pEffect, pEffect = nullptr; }
	char id[40];
	char path[40];
	Effect2D* pEffect{ nullptr };
};

class SoundManager : public Singleton<SoundManager>
{
public:
	~SoundManager();

	void Initialize(SaveHandler* pSaveHandler);
	void DrawInterface();
	void Save(SaveHandler* pSaveHandler);

	void PlaySoundEffect(const std::string& id);
	void PlayMusic(const std::string& id);
	void StopMusic();

private:
	std::vector<MusicObject*> m_pMusicObjects{};
	std::vector<EffectObject*> m_pEffectObjects{};
};

