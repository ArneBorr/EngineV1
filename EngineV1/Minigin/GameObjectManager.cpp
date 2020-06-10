#include "MiniginPCH.h"
#include "GameObjectManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"
#include "PlayerScript.h"
#include "AllowOneWay.h"
#include "Components.h"

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
		if (ImGui::Button("Add Character"))
			CreateCharacter();
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
	auto pScene = SceneManager::GetInstance()->GetCurrentScene();
	if (pScene)
	{
		pScene->InitialAdd(new GameObject("GameObject"));
	}
}

void GameObjectManager::CreateCharacter()
{
	auto pScene = SceneManager::GetInstance()->GetCurrentScene();
	if (pScene)
	{
		auto pGameObject = new GameObject("Character");
		pGameObject->SetScene(pScene);

		auto pTransform = new TransformComponent(pGameObject);
		pTransform->LoadSettings("Player");
		pGameObject->SetTransform(pTransform);

		auto pMovement = new MovementComponent(pGameObject);
		pMovement->LoadSettings("Player");
		pGameObject->AddComponent(pMovement);

		auto pRigidbody = new RigidbodyComponent(pGameObject);
		pRigidbody->LoadSettings("Player");
		pGameObject->AddComponent(pRigidbody);
		pGameObject->SetRigidbody(pRigidbody);

		auto pBoxCollider = new BoxColliderComponent(pGameObject, pRigidbody);
		pBoxCollider->LoadSettings("Player");
		pGameObject->AddComponent(pBoxCollider);

		auto pScriptComponent0 = new ScriptComponent(pGameObject);
		pScriptComponent0->SetScript(new PlayerScript());
		pGameObject->AddComponent(pScriptComponent0);

		auto pScriptComponent1 = new ScriptComponent(pGameObject);
		pScriptComponent1->SetScript(new AllowOneWay());
		pGameObject->AddComponent(pScriptComponent1);

		auto pAnimator = new AnimatorControllerComponent(pGameObject);
		pAnimator->LoadSettings("Player");
		pGameObject->AddComponent(pAnimator);

		pScene->InitialAdd(pGameObject);
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
