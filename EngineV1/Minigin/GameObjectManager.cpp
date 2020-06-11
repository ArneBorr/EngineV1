#include "MiniginPCH.h"
#include "GameObjectManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"
#include "PlayerScript.h"
#include "AllowOneWay.h"
#include "Components.h"
#include "IdleBehaviour.h"
#include "RunBehaviour.h"

void GameObjectManager::Initialize()
{
	m_pScripts.push_back(new PlayerScript());
	m_pScripts.push_back(new AllowOneWay());

	m_pBehaviours.push_back(new IdleBehaviour());
	m_pBehaviours.push_back(new RunBehaviour());
}

GameObjectManager::~GameObjectManager()
{
	for (auto script : m_pScripts)
	{
		delete script;
		script = nullptr;
	}
	m_pScripts.clear();

	for (auto behaviour : m_pBehaviours)
	{
		delete behaviour;
		behaviour = nullptr;
	}
	m_pBehaviours.clear();


	if (m_pSelectedScript)
	{
		delete m_pSelectedScript;
		m_pSelectedScript = nullptr;
	}

	if (m_pSelectedBehaviour)
	{
		delete m_pSelectedBehaviour;
		m_pSelectedBehaviour = nullptr;
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
	using namespace ImGui;
	const auto windowSize{ GetWindowSize() };
	if (BeginTabItem("Objects"))
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
		Columns(3, " ");

		//Show prefabs
		BeginChild("Prefabs", ImVec2(windowSize.x / 3.f, windowSize.y), true, window_flags);
		if (Button("Add Empty GameObject"))
			CreateEmptyGameObject();
		if (Button("Add Character"))
			CreateCharacter();
		EndChild();

		// Show all possible Scripts
		ImGui::NextColumn();
		BeginChild("Scripts", ImVec2(windowSize.x / 3.f, windowSize.y), true, window_flags);
		for (auto script : m_pScripts)
		{
			PushID(script);
			Button(script->GetName().c_str());
			//Allow to be dragged
			if (BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				if (m_pSelectedScript)
					delete m_pSelectedScript;

				m_pSelectedScript = CreateScript(script->GetName());
				SetDragDropPayload("Script", &script->GetName(), script->GetName().size(), ImGuiCond_Once);
				Text(script->GetName().c_str());
				EndDragDropSource();
			}
			PopID();
;		}
		EndChild();

		//Show Behaviours
		NextColumn();
		BeginChild("Behaviours", ImVec2(windowSize.x / 3.f, windowSize.y), true, window_flags);

		for (auto behaviour : m_pBehaviours)
		{
			PushID(behaviour);

			Button(behaviour->GetName().c_str());
			//Allow to be dragged
			if (BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				if (m_pSelectedBehaviour)
					delete m_pSelectedBehaviour;

				m_pSelectedBehaviour = CreateBehaviour(behaviour->GetName());
				SetDragDropPayload("SelectedBehaviour", &behaviour->GetName(), behaviour->GetName().size(), ImGuiCond_Once);
				Text(behaviour->GetName().c_str());
				EndDragDropSource();
			}

			PopID();
		}
		EndChild();

		Columns(1);
		EndTabItem();
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

Behaviour* GameObjectManager::CreateBehaviour(const std::string& name)
{
	if (name == "IdleBehaviour")
		return new IdleBehaviour();
	else if (name == "RunBehaviour")
		return new RunBehaviour();

	std::printf("GameObjectManager::CreateScript() : Behaviour not found\n");
	return nullptr;
}

Script* GameObjectManager::GetAndRemoveSelectedScript()
{
	Script* temp = m_pSelectedScript;
	m_pSelectedScript = nullptr;
	return temp;
}

Behaviour* GameObjectManager::GetAndRemoveSelectedBehaviour()
{
	Behaviour* temp = m_pSelectedBehaviour;
	m_pSelectedBehaviour = nullptr;
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
