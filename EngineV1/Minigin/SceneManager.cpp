#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "GameObject.h"


SceneManager::~SceneManager()
{
	for (unsigned int i{}; i < m_pScenes.size(); i++)
	{
		delete m_pScenes[i];
		m_pScenes[i] = nullptr;
	}
}

void SceneManager::DrawInterface()
{
	ImGui::SetNextWindowPos({ 0,0 }, ImGuiCond_Always);
	auto windowSize = GameInfo::GetInstance()->GetWindowSize();
	static float widthRatio = 0.225f;
	static float heightRatio = 1.f;
	ImGui::SetNextWindowSize({ windowSize.x * widthRatio, windowSize.x * heightRatio });

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	ImGui::Begin("SceneManager");


	if (ImGui::BeginTabBar("SceneManager", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Scenes"))
		{
			ImGui::EndTabItem();
			ImGui::Button("AddScene");
		}

		if (ImGui::BeginTabItem("CurrentScene"))
		{
			ImGui::EndTabItem();
			
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					auto pObject = new GameObject(std::move(*(GameObject*)(payload->Data)));
					m_pCurrentScene->AddObject(pObject);
				}

				ImGui::EndDragDropTarget();
			}

			m_pCurrentScene->DrawInterface();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}

void SceneManager::AddScene(Scene* scene)
{
	m_pScenes.push_back(scene);
	m_pCurrentScene = scene;
}

void SceneManager::Initialize()
{
	for (auto& scene : m_pScenes)
	{
		scene->Initialize();
	}
}

void SceneManager::Update(float elapsedSec)
{
	for (auto& scene : m_pScenes)
	{
		scene->Update(elapsedSec);
	}
}

void SceneManager::Render()
{
	for (const auto& scene : m_pScenes)
	{
		scene->Render();
	}
}
