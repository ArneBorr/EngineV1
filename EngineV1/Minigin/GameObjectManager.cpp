#include "MiniginPCH.h"
#include "GameObjectManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"
#include "PlayerScript.h"
#include "AllowOneWay.h"
#include "Components.h"
#include "Behaviours.h"

void GameObjectManager::Initialize()
{
	m_pScripts.push_back(new PlayerScript());
	m_pScripts.push_back(new AllowOneWay());

	m_pBehaviours.push_back(new IdleBehaviour());
	m_pBehaviours.push_back(new RunBehaviour());
	m_pBehaviours.push_back(new JumpBehaviour());
	m_pBehaviours.push_back(new ShootBehaviour());
	m_pBehaviours.push_back(new BubbleFloatBehaviour());
	m_pBehaviours.push_back(new BubbleHitEnemyBehaviour());
	m_pBehaviours.push_back(new BubblePopBehaviour());
	m_pBehaviours.push_back(new BubbleShootBehaviour());
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
		if (Button("Add Bubble"))
			CreateBubble();
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

		auto pRigidbody = new RigidbodyComponent(pGameObject);
		pRigidbody->LoadSettings("Player");
		pGameObject->AddComponent(pRigidbody);
		pGameObject->SetRigidbody(pRigidbody);

		auto pBoxCollider = new BoxColliderComponent(pGameObject, pRigidbody);
		pBoxCollider->LoadSettings("Player");
		pGameObject->AddComponent(pBoxCollider);

		auto pScriptComponent1 = new ScriptComponent(pGameObject);
		pScriptComponent1->SetScript(new AllowOneWay());
		pGameObject->AddComponent(pScriptComponent1);

		pScene->InitialAdd(pGameObject);
	}
}

void GameObjectManager::CreateBubble()
{
	auto pScene = SceneManager::GetInstance()->GetCurrentScene();
	if (pScene)
	{
		auto pGameObject = new GameObject("Bubble");
		pGameObject->SetScene(pScene);

		auto pTransform = new TransformComponent(pGameObject);
		pTransform->LoadSettings("Bubble");
		pGameObject->SetTransform(pTransform);

		auto pRigidbody = new RigidbodyComponent(pGameObject);
		pRigidbody->LoadSettings("Bubble");
		pGameObject->AddComponent(pRigidbody);
		pGameObject->SetRigidbody(pRigidbody);

		auto pBoxCollider = new BoxColliderComponent(pGameObject, pRigidbody);
		pBoxCollider->LoadSettings("Bubble");
		pGameObject->AddComponent(pBoxCollider);

		auto pFSM = new FSMComponent(pGameObject);
		pFSM->LoadSettings("Bubble");
		pGameObject->AddComponent(pFSM);

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
	else if (name == "JumpBehaviour")
		return new JumpBehaviour();
	else if (name == "ShootBehaviour")
		return new ShootBehaviour();
	else if (name == "BubbleFloatBehaviour")
		return new BubbleFloatBehaviour();
	else if (name == "BubbleHitEnemyBehaviour")
		return new BubbleHitEnemyBehaviour();
	else if (name == "BubblePopBehaviour")
		return new BubblePopBehaviour();
	else if (name == "BubbleShootBehaviour")
		return new BubbleShootBehaviour();

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
