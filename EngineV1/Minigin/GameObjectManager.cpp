#include "MiniginPCH.h"
#include "GameObjectManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"



GameObjectManager::~GameObjectManager()
{
	delete m_pSelectedGameObject;
	m_pSelectedGameObject = nullptr;
}

void GameObjectManager::DrawInterface() const
{
	static float widthRatioBottomWindow = 0.398f;
	static float heightRatioBottomWindow = 0.26f;
	auto windowSize =  GameInfo::GetInstance()->GetWindowSize();

	ImGui::SetNextWindowBgAlpha(1.f);
	ImGui::SetNextWindowPos({  windowSize.x * widthRatioBottomWindow / 2.f, windowSize.y - windowSize.y * heightRatioBottomWindow }, ImGuiCond_Always);
	ImGui::SetNextWindowSize({ windowSize.x *  (1 - widthRatioBottomWindow), windowSize.y * heightRatioBottomWindow });

	ImGui::Begin("Objects");
	if (ImGui::Button("Add Empty GameObject"))
	{
		CreateEmptyGameObject();
	}
	ImGui::End();

	static float widthRatioRightWindow = 0.2f;
	static float heightRatioRightWindow = 1.f;
	ImGui::SetNextWindowPos({ windowSize.x - windowSize.x * widthRatioRightWindow, 0 }, ImGuiCond_Always);
	ImGui::SetNextWindowSize({ windowSize.x * widthRatioRightWindow, windowSize.y * heightRatioRightWindow });
	ImGui::SetNextWindowBgAlpha(1.f);

	ImGui::Begin("GameObject Components");
	
	if (m_pSelectedGameObject)
	{
		m_pSelectedGameObject->DrawInterfaceComponents();
	}

	ImGui::End();
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
