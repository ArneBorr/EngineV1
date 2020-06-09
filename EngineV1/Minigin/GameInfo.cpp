#include "MiniginPCH.h"
#include "GameInfo.h"
#include "imgui.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"
#include "SaveHandler.h"

const Vector2f GameInfo::m_WindowSize = Vector2f{ 1280.f, 720.f };

GameInfo::~GameInfo()
{
	delete m_pSaveHandler;
	m_pSaveHandler = nullptr;
}

void GameInfo::Initialize(SaveHandler* pSaveHandler)
{
	m_pSaveHandler = pSaveHandler;
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

	//std::cout << 60.f / m_ElapsedTime << "\n";
}

void GameInfo::DrawInterface()
{
	if (ImGui::BeginTabItem("TaskBar"))
	{
		if (ImGui::Button("Play"))
		{
			m_IsPlaying = !m_IsPlaying;
			if (m_IsPlaying)
				SceneManager::GetInstance()->StartPlaying();
		}
		const char* playing = m_IsPlaying ? "Playing" : "Not Playing";
		ImGui::SameLine();
		ImGui::Text(playing);


		if (ImGui::Button("Save"))
		{
			SceneManager::GetInstance()->SaveScenes(m_pSaveHandler);
			InputManager::GetInstance()->SaveInput(m_pSaveHandler);
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
