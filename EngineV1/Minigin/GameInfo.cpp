#include "MiniginPCH.h"
#include "GameInfo.h"
#include "imgui.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"
#include "SaveHandler.h"
#include "Renderer.h"
#include "SoundManager.h"

GameInfo::~GameInfo()
{
	delete m_pSaveHandler;
	m_pSaveHandler = nullptr;
}

void GameInfo::Initialize(SaveHandler* pSaveHandler)
{
	m_pSaveHandler = pSaveHandler;

#if defined(GAME)
	m_IsFullscreen = true;
	m_IsPlaying = true;
	SceneManager::GetInstance()->GetCurrentScene()->ChangeGameobjectsToFullscreen();
#endif
}

void GameInfo::StartTimer()
{
	m_CurrFrame = std::chrono::high_resolution_clock::now();
	m_PrevFrame = m_CurrFrame;
}

void GameInfo::Update()
{
	m_PrevFrame = m_CurrFrame;
	m_CurrFrame = std::chrono::high_resolution_clock::now();
	m_ElapsedTime = std::chrono::duration<float>(m_CurrFrame - m_PrevFrame).count();
}

void GameInfo::DrawInterface()
{
	if (ImGui::BeginTabItem("TaskBar"))
	{
		if (ImGui::Button("Play"))
		{
			m_IsPlaying = !m_IsPlaying;
			if (m_IsPlaying)
			{
				SceneManager::GetInstance()->StartPlaying();
				SoundManager::GetInstance()->PlayMusic("Background");
			}
			else
			{
				SceneManager::GetInstance()->StopPlaying();
				SoundManager::GetInstance()->StopMusic();
			}
		}
		const char* playing = m_IsPlaying ? "Playing" : "Not Playing";
		ImGui::SameLine();
		ImGui::Text(playing);

		ImGui::SameLine(670.f);
		ImGui::Text("FPS: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(int(60.f / m_ElapsedTime)).c_str());

		if (ImGui::Button("Save"))
		{
			SceneManager::GetInstance()->SaveScenes(m_pSaveHandler);
			InputManager::GetInstance()->SaveInput(m_pSaveHandler);
			SoundManager::GetInstance()->Save(m_pSaveHandler);
		}

		if (ImGui::Button("FullScreen"))
		{
			m_IsFullscreen = true;
			auto windowSize = GetWindowSize();
			const Vector4f editorDimensions = Renderer::GetInstance()->GetEditorDimensions();

			const float scaleRatioX = windowSize.x / editorDimensions.z;
			const float scaleRatioY = windowSize.y / editorDimensions.w;
			SceneManager::GetInstance()->AdaptToFullscreen({scaleRatioX, scaleRatioY});
		}

		ImGui::EndTabItem();
	}
}

float GameInfo::GetElapsedSeconds()
{
	return m_ElapsedTime;
}
