#include "MiniginPCH.h"
#include "GameInfo.h"
#include "imgui.h"
#include "SceneManager.h"
#include "Scene.h"


const Vector2f GameInfo::m_WindowSize = Vector2f{ 1280.f, 720.f };

void GameInfo::Start()
{
	m_CurrFrame = std::chrono::high_resolution_clock::now();
	m_PrevFrame = m_CurrFrame;
}

void GameInfo::Update()
{
	m_PrevFrame = m_CurrFrame;
	m_CurrFrame = std::chrono::high_resolution_clock::now();
	m_ElapsedTime = std::chrono::duration<float>(m_CurrFrame - m_PrevFrame).count();

	//std::cout << 60.f / m_ElapsedTime << "\n";
}

void GameInfo::DrawInterface()
{
	if (ImGui::BeginTabItem("TaskBar"))
	{
		if (ImGui::Button("Play"))
		{
			m_IsPlaying = !m_IsPlaying;
		}
		const char* playing = m_IsPlaying ? "Playing" : "Not Playing";
		ImGui::SameLine();
		ImGui::Text(playing);


		if (ImGui::Button("Save"))
		{
			SceneManager::GetInstance()->SaveScenes();
		}

		if (ImGui::Button("FullScreen"))
		{
			m_IsFullscreen = true;
			SceneManager::GetInstance()->GetCurrentScene()->ChangeGameobjectsToFullscreen();
		}

		ImGui::EndTabItem();
	}
}

float GameInfo::GetElapsedSeconds()
{
	return m_ElapsedTime;
}
