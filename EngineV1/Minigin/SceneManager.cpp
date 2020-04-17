#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "imgui.h"


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

	ImGui::ShowDemoWindow();
	
	ImGui::SetNextWindowPos({ 0,0 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 400, 400 });
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	ImGui::Begin("SceneManager");
	if (ImGui::BeginTabBar("SceneManager", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Scenes"))
		{
			ImGui::Button("AddScene");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("CurrentScene"))
		{
			m_pCurrentScene->DrawInterface();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
	/*ImGui::Button("ok");
	ImGui::Button("ok");
	ImGui::Button("ok");
	ImGui::Button("ok");
	ImGui::Button("ok");
	ImGui::Button("ok");
	ImGui::Button("ok");
	ImGui::Button("ok");*/
	
	//ImGui::ShowDemoWindow();
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
