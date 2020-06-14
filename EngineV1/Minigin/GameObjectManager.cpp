#include "MiniginPCH.h"
#include "GameObjectManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"
#include "Scripts.h"
#include "Components.h"
#include "Behaviours.h"
#include "SaveHandler.h"

void GameObjectManager::Initialize()
{
	m_pSaveHandlerPrefabs = new SaveHandler();
	m_pSaveHandlerPrefabs->LoadPrefabNames(m_Prefabs);

	m_pScripts.push_back(new AllowOneWay());
	m_pScripts.push_back(new PickUp());
	m_pScripts.push_back(new Projectile());
	m_pScripts.push_back(new HUD());
	m_pScripts.push_back(new LinkToSceneOverhead());
	m_pScripts.push_back(new MainMenu());
	m_pScripts.push_back(new EnemyCounter());

	m_pBehaviours.push_back(new IdleBehaviour());
	m_pBehaviours.push_back(new RunBehaviour());
	m_pBehaviours.push_back(new JumpBehaviour());
	m_pBehaviours.push_back(new AttackBehaviour());
	m_pBehaviours.push_back(new HitBehaviour());
	m_pBehaviours.push_back(new BubbleFloatBehaviour());
	m_pBehaviours.push_back(new BubbleHitEnemyBehaviour());
	m_pBehaviours.push_back(new BubblePopBehaviour());
	m_pBehaviours.push_back(new BubbleShootBehaviour());
	m_pBehaviours.push_back(new EnemyMove());
	m_pBehaviours.push_back(new EnemyJump());
	m_pBehaviours.push_back(new EnemyScan());
	m_pBehaviours.push_back(new LaunchEnemy());
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
	m_Prefabs.clear();


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

	delete m_pSaveHandlerPrefabs;
	m_pSaveHandlerPrefabs = nullptr;
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
	const float offset{ 35.f };
	if (BeginTabItem("Objects"))
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
		Columns(3, " ");
		//Show prefabs
		auto pScene = SceneManager::GetInstance()->GetCurrentScene();
		BeginChild("Prefabs", ImVec2(windowSize.x / 3.f, windowSize.y - offset), true, window_flags);
		if (pScene)
		{
			if (Button("Add Empty GameObject"))
				pScene->InitialAdd(CreateEmptyGameObject());

			for (auto it = m_Prefabs.begin(); it != m_Prefabs.end();)
			{
				//Add prefab
				PushID(&(*it));
				Button((*it).c_str());
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
					pScene->InitialAdd(m_pSaveHandlerPrefabs->LoadPrefab(pScene, (*it)));
				
				//Drag prefab
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					auto ok = &it;
					UNREFERENCED_PARAMETER(ok);
					std::string text = (*it);
					ImGui::SetDragDropPayload("Prefab", &text, sizeof(text), ImGuiCond_Once);
					ImGui::Text((*it).c_str()); 
					ImGui::EndDragDropSource();
				}

				//Delete prefab
				SameLine();
				Button("YOINK") && ImGui::IsMouseDoubleClicked(0);
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
				{
					m_pSaveHandlerPrefabs->ErasePrefab((*it));
					it = m_Prefabs.erase(it);
				}
				else
					++it;

				PopID();
			}
		}	
		EndChild();
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
			{
				GameObject* pGameObject = GameObjectManager::GetInstance()->GetSelectedGameObject();
				auto it = std::find(m_Prefabs.begin(), m_Prefabs.end(), pGameObject->GetName());
				if (it == m_Prefabs.end())
				{
					m_Prefabs.push_back(pGameObject->GetName());
					m_pSaveHandlerPrefabs->SavePrefab(pGameObject);
				}	
			}
			ImGui::EndDragDropTarget();
		}

		// Show all possible Scripts
		ImGui::NextColumn();
		BeginChild("Scripts", ImVec2(windowSize.x / 3.f, windowSize.y - offset), true, window_flags);
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
		BeginChild("Behaviours", ImVec2(windowSize.x / 3.f, windowSize.y - offset), true, window_flags);

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

GameObject* GameObjectManager::CreateEmptyGameObject() const
{
	return new GameObject("GameObject");	
}

Script* GameObjectManager::CreateScript(const std::string& name)
{
	//Ugly :)
	if (name == "AllowOneWay")
		return new AllowOneWay();
	else if (name == "PickUp")
		return new PickUp();
	else if (name == "Projectile")
		return new Projectile();
	else if (name == "HUD")
		return new HUD();
	else if (name == "LinkToSceneOverhead")
		return new LinkToSceneOverhead();
	else if (name == "MainMenu")
		return new MainMenu();
	else if (name == "EnemyCounter")
		return new EnemyCounter();

	std::printf("GameObjectManager::CreateScript() : Script not found");
	return nullptr;
}

GameObject* GameObjectManager::GetPrefab(Scene* pScene, const std::string& name) const
{
	auto temp = m_pSaveHandlerPrefabs->LoadPrefab(pScene, name);
	if (!temp)
	{
		std::printf(" GameObjectManager::GetPrefab() : Prefab Not Found\n");
		return nullptr;
	}

	return temp;
}

GameObject* GameObjectManager::SpawnPrefab(Scene* pScene, const std::string& name, const Vector2f pos) const
{
	auto prefab = GetPrefab(pScene, name);
	if (prefab)
	{
		prefab->GetTransform()->SetPosition(pos);
		pScene->AddChild(prefab);
		prefab->Initialize();
	}

	return prefab;
}

Behaviour* GameObjectManager::CreateBehaviour(const std::string& name)
{
	if (name == "IdleBehaviour")
		return new IdleBehaviour();
	else if (name == "RunBehaviour")
		return new RunBehaviour();
	else if (name == "JumpBehaviour")
		return new JumpBehaviour();
	else if (name == "AttackBehaviour")
		return new AttackBehaviour();
	else if (name == "BubbleFloatBehaviour")
		return new BubbleFloatBehaviour();
	else if (name == "BubbleHitEnemyBehaviour")
		return new BubbleHitEnemyBehaviour();
	else if (name == "BubblePopBehaviour")
		return new BubblePopBehaviour();
	else if (name == "BubbleShootBehaviour")
		return new BubbleShootBehaviour();
	else if (name == "EnemyMove")
		return new EnemyMove();
	else if (name == "EnemyJump")
		return new EnemyJump();
	else if (name == "EnemyScan")
		return new EnemyScan();
	else if (name == "LaunchEnemy")
		return new LaunchEnemy();
	else if (name == "HitBehaviour")
		return new HitBehaviour();

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
