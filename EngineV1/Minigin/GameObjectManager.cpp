#include "MiniginPCH.h"
#include "GameObjectManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"


void GameObjectManager::DrawInterface()  const
{
	ImGui::Begin("GameObjectManager");
	if (ImGui::Button("Add Empty GameObject"))
	{
		CreateEmptyGameObject();
	}
	ImGui::End();
}

void GameObjectManager::CreateEmptyGameObject() const
{
	auto scene = SceneManager::GetInstance()->GetCurrentScene();
	if (scene)
	{
		scene->Add(new GameObject());
	}
}
