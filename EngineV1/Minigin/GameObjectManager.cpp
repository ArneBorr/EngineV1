#include "MiniginPCH.h"
#include "GameObjectManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"




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
