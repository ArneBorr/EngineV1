#include "MiniginPCH.h"
#include "SoundManager.h"
#include "imgui.h"
#include "Music2D.h"
#include "Effect2D.h"
#include "SaveHandler.h"
#include "ResourceManager.h"

SoundManager::~SoundManager()
{
	for (auto music : m_pMusicObjects)
	{
		delete music;
		music = nullptr;
	}
	for (auto effect : m_pEffectObjects)
	{
		delete effect;
		effect = nullptr;
	}
}

void SoundManager::Initialize(SaveHandler* pSaveHandler)
{
	pSaveHandler->LoadSound(m_pMusicObjects, m_pEffectObjects);
}

void SoundManager::DrawInterface()
{
	using namespace ImGui;

	if (BeginTabItem("Sound"))
	{
		Separator();
		Text("Music");
		Separator();

		for (auto it = m_pMusicObjects.begin(); it != m_pMusicObjects.end();)
		{
			PushID(&it);
			MusicObject* pMusicObject = (*it);
			Text(pMusicObject->id);
			Separator();

			InputText("Name", pMusicObject->id, IM_ARRAYSIZE(pMusicObject->id));
			InputText("Path", pMusicObject->path, IM_ARRAYSIZE(pMusicObject->path));
			SameLine();
			if (Button("Update"))
			{
				delete pMusicObject->pMusic;
				pMusicObject->pMusic = ResourceManager::GetInstance()->LoadMusic(pMusicObject->path);
			}

			if (Button("X"))
			{
				delete pMusicObject;
				pMusicObject = nullptr;
				it = m_pMusicObjects.erase(it);
			}
			else
				++it;
		
			Separator();
			PopID();
		}

		PushID(0);
		static char musicID[40]{"New Sound"};
		InputText(" " , musicID, IM_ARRAYSIZE(musicID));
		SameLine();
		if (Button("Add"))
			m_pMusicObjects.push_back(new MusicObject{ musicID });
		PopID();

		Separator();
		Text("Effects");
		Separator();

		for (auto it = m_pEffectObjects.begin(); it != m_pEffectObjects.end();)
		{
			PushID(&it);
			EffectObject* pEffectObject = (*it);
			Text(pEffectObject->id);
			Separator();

			InputText("Name", pEffectObject->id, IM_ARRAYSIZE(pEffectObject->id));
			InputText("Path", pEffectObject->path, IM_ARRAYSIZE(pEffectObject->path));
			SameLine();
			if (Button("Update"))
			{
				delete pEffectObject->pEffect;
				pEffectObject->pEffect = ResourceManager::GetInstance()->LoadSoundEffect(pEffectObject->path);;
			}

			if (Button("X"))
			{
				delete pEffectObject;
				pEffectObject = nullptr;
				it = m_pEffectObjects.erase(it);
			}
			else
				++it;

			Separator();
			PopID();
		}

		PushID(1);
		static char effectText[40]{ "New Effect" };
		InputText(" ", effectText, IM_ARRAYSIZE(effectText));
		SameLine();
		if (Button("Add"))
			m_pEffectObjects.push_back(new EffectObject{ effectText });
		PopID();

		EndTabItem();
	}
}

void SoundManager::Save(SaveHandler* pSaveHandler)
{
	pSaveHandler->SaveSound(m_pMusicObjects, m_pEffectObjects);
}

void SoundManager::PlaySoundEffect(const std::string& id)
{
	auto it = std::find_if(m_pEffectObjects.begin(), m_pEffectObjects.end(), [id](EffectObject* pEffect) { return id == pEffect->id;  });
	if (it != m_pEffectObjects.end())
		Mix_PlayChannel(-1, (*it)->pEffect->GetSDLChunk(), 0);
	else
		std::printf("SoundManager::PlaySoundEffect() : Effect not Found!");
}

void SoundManager::PlayMusic(const std::string& id)
{
	auto it = std::find_if(m_pMusicObjects.begin(), m_pMusicObjects.end(), [id](MusicObject* pMusic) { return id == pMusic->id;  });
	if (it != m_pMusicObjects.end())
		Mix_PlayMusic((*it)->pMusic->GetSDLMusic(), -1);
	else
		std::printf("SoundManager::PlaySoundEffect() : Effect not Found!");
}

void SoundManager::StopMusic()
{
	Mix_HaltMusic();
}

