#include "MiniginPCH.h"
#include "GameObjectManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"
#include "PlayerScript.h"

void GameObjectManager::Initialize()
{
	m_pScripts.push_back(new PlayerScript());
}

GameObjectManager::~GameObjectManager()
{
	for (auto script : m_pScripts)
	{
		delete script;
		script = nullptr;
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

void GameObjectManager::DrawInterface2() const
{
	if (ImGui::BeginTabItem("Objects"))
	{
		if (ImGui::Button("Add Empty GameObject"))
		{
			CreateEmptyGameObject();
		}

		// Show all possible Scripts
		for (auto script : m_pScripts)
		{
			ImGui::PushID(script);
			ImGui::Button(script->GetName().c_str());
			//Allow to be dragged
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload(script->GetName().c_str(), script, sizeof(Script), ImGuiCond_Once);
				ImGui::Text("PlayerScript");
				ImGui::EndDragDropSource();
			}
			ImGui::PopID();
;		}


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
