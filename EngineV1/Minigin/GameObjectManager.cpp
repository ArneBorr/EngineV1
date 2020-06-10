#include "MiniginPCH.h"
#include "GameObjectManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"
#include "PlayerScript.h"
#include "AllowOneWay.h"

void GameObjectManager::Initialize()
{
	m_pScripts.push_back(new PlayerScript());
	m_pScripts.push_back(new AllowOneWay());
}

GameObjectManager::~GameObjectManager()
{
	for (auto script : m_pScripts)
	{
		delete script;
		script = nullptr;
	}

	if (m_pSelectedScript)
	{
		delete m_pSelectedScript;
		m_pSelectedScript = nullptr;
	}
}

void GameObjectManager::DrawInterface1() const
{
	if (ImGui::BeginTabItem("Inspector"))
	{
		if (m_pSelectedGameObject)
		{
			m_pSelectedGameObject->DrawInterfaceComponents();
		}

		ImGui::EndTabItem();
	}
}

void GameObjectManager::DrawInterface2()
{
	const Vector2f windowSize{ 200, 150 };
	if (ImGui::BeginTabItem("Objects"))
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;

		//Show prefabs
		ImGui::BeginChild("Prefabs", ImVec2(windowSize.x, windowSize.y), true, window_flags);
		if (ImGui::Button("Add Empty GameObject"))
			CreateEmptyGameObject();
		ImGui::EndChild();

		// Show all possible Scripts
		ImGui::SameLine();
		ImGui::BeginChild("Scripts", ImVec2(windowSize.x, windowSize.y), true, window_flags);
		for (auto script : m_pScripts)
		{
			ImGui::PushID(script);
			ImGui::Button(script->GetName().c_str());
			//Allow to be dragged
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				if (m_pSelectedScript)
					delete m_pSelectedScript;

				m_pSelectedScript = CreateScript(script->GetName());
				ImGui::SetDragDropPayload("Script", &script->GetName(), script->GetName().size(), ImGuiCond_Once);
				ImGui::Text(script->GetName().c_str());
				ImGui::EndDragDropSource();
			}
			ImGui::PopID();
;		}
		ImGui::EndChild();

		ImGui::EndTabItem();
	}
}

void GameObjectManager::SetSelectedGameObject(GameObject* pGameObject)
{
	m_pSelectedGameObject = pGameObject;
}

void GameObjectManager::CreateEmptyGameObject() const
{
	auto scene = SceneManager::GetInstance()->GetCurrentScene();
	if (scene)
	{
		scene->InitialAdd(new GameObject("GameObject"));
	}
}

Script* GameObjectManager::CreateScript(const std::string& name)
{
	if (name == "PlayerScript")
		return new PlayerScript();
	else if (name == "AllowOneWay")
		return new AllowOneWay();
	
	std::printf("GameObjectManager::CreateScript() : Script not found");
	return nullptr;
}

Script* GameObjectManager::GetAndRemoveSelectedScript()
{
	Script* temp = m_pSelectedScript;
	m_pSelectedScript = nullptr;
	return temp;
}

Script* GameObjectManager::GetScript(const std::string & name)
{
	for (auto script : m_pScripts)
	{
		if (script->GetName() == name)
			return script;
	}

	std::printf("GameObjectManager::GetScript() : Script not found");
	return nullptr;
}
