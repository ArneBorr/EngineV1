#include "MiniginPCH.h"
#include "FSMComponent.h"
#include "imgui.h"
#include "Sprite.h"
#include "Behaviour.h"
#include "TextureComponent.h"
#include "Blackboard.h"

using namespace ImGui;

FSMComponent::FSMComponent(GameObject* pObject)
	: BaseComponent(pObject, "FSMComponent")
	, m_pBlackboard{ new Blackboard() }
{
}

FSMComponent::~FSMComponent()
{
	for (auto sprite : m_pSprites)
	{
		delete sprite;
		sprite = nullptr;
	}
	m_pSprites.clear();

	for (auto behaviour : m_pBehaviours)
	{
		delete behaviour;
		behaviour = nullptr;
	}
	m_pBehaviours.clear();

	delete m_pBlackboard;
	m_pBlackboard = nullptr;
}

void FSMComponent::Initialize()
{
	if (m_pBehaviours.size() > 0)
	{
		m_pCurrentBehaviour = m_pBehaviours[m_StartingBehaviourIndex];
		for (auto behaviour : m_pBehaviours)
			behaviour->Initialize();

		m_pCurrentBehaviour->Enter();
	}
}

void FSMComponent::Render()
{
	if (m_IsPaused)
		return;

	//Only Render Behaviour on top
	if (m_pOnTopBehaviour)
		m_pOnTopBehaviour->Render();
	else if (m_pCurrentBehaviour)
		m_pCurrentBehaviour->Render();
}

void FSMComponent::Update(float elapsedSec)
{
	if (!GameInfo::GetInstance()->IsPlaying() || !m_pCurrentBehaviour || m_IsPaused)
		return;

	Behaviour* pBehaviour = m_pCurrentBehaviour->HandleInput();
	//If state wants to change
	if (pBehaviour)
	{
		//Check if new state goes on top of current state
		if (pBehaviour->IsOnTop())
		{
			//Dont add when there is already a state on top
			if (!m_pOnTopBehaviour)
			{
				m_pOnTopBehaviour = pBehaviour;
				m_pOnTopBehaviour->Enter();
			}
		}
		//Swap Current State
		else
		{
			m_pCurrentBehaviour->Exit();
			m_pCurrentBehaviour = pBehaviour;
			m_pCurrentBehaviour->Enter();
		}
	}

	if (m_pOnTopBehaviour)
	{
		auto temp = m_pOnTopBehaviour->HandleInput();
		//If currentstate is over, reset
		if (!temp)
		{
			m_pOnTopBehaviour->Exit();
			m_pOnTopBehaviour = nullptr;
		}
	}

	m_pCurrentBehaviour->Update(elapsedSec);

	if (m_pOnTopBehaviour)
		m_pOnTopBehaviour->Update(elapsedSec);
}

void FSMComponent::DrawInterface()
{
	SetNextItemOpen(true, ImGuiCond_Once);

	bool open = TreeNode(&GetName().front());
	HandleDrag();

	if (open)
	{
		Separator();

		if (Button("Window"))
			m_IsWindowOpen = !m_IsWindowOpen;

		if (m_IsWindowOpen)
		{
			SetNextWindowBgAlpha(1.f);
			SetNextWindowPos({ 200, 200 }, ImGuiCond_Appearing);
			SetNextWindowSize({ 500, 500 }, ImGuiCond_Appearing);
			Begin("Uber Duper Cool Finite State Machine");
			BeginTabBar(" ");


			if (BeginTabItem("FSM"))
			{
				DrawFSMTab();
				EndTabItem();
			}

			if (BeginTabItem("Sprites"))
			{
				DrawSpriteTab();
				EndTabItem();
			}

			EndTabBar();
			End();
		}	
		
		TreePop();
	}

	HandleDrop();
}

void FSMComponent::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("StartingState", IntToXMLChar(doc, int(m_StartingBehaviourIndex))));
	//Sprites
	rapidxml::xml_node<>* pMainSpritesNode = doc->allocate_node(rapidxml::node_element, "Sprites");
	for (auto sprite : m_pSprites)
	{
		rapidxml::xml_node<>* spriteNode = doc->allocate_node(rapidxml::node_element, "Sprite");
		sprite->SaveAttributes(doc, spriteNode);
		pMainSpritesNode->append_node(spriteNode);
	}
	node->append_node(pMainSpritesNode);

	//Behaviours
	rapidxml::xml_node<>* pMainBehavioursNode = doc->allocate_node(rapidxml::node_element, "Behaviours");
	for (auto behaviour : m_pBehaviours)
	{
		rapidxml::xml_node<>* pBehaviourNode = doc->allocate_node(rapidxml::node_element, "Behaviour");
		behaviour->SaveAttributes(doc, pBehaviourNode);
		pMainBehavioursNode->append_node(pBehaviourNode);
	}
	node->append_node(pMainBehavioursNode);
}

void FSMComponent::SetAttributes(rapidxml::xml_node<>* node)
{
	int i{};
	for (rapidxml::xml_node<>* behaviourNode = node->first_node(); behaviourNode != 0; behaviourNode = behaviourNode->next_sibling())
	{
		m_pBehaviours[i]->SetAttributes(behaviourNode);
		++i;
	}

	if (m_pBehaviours.size() > 0)
		m_pCurrentBehaviour = m_pBehaviours[m_StartingBehaviourIndex];
}

void FSMComponent::OnNotify(const std::string& event, GameObject* pObject, GameObject* trigger)
{
	if (m_pOnTopBehaviour)
		m_pOnTopBehaviour->OnNotify(event, pObject, trigger);

	if (m_pCurrentBehaviour)
		m_pCurrentBehaviour->OnNotify(event, pObject, trigger);
}

void FSMComponent::SetBehaviours(const std::vector<Behaviour*>& pBehaviours)
{
	m_pBehaviours = pBehaviours;
}

void FSMComponent::SetSprites(const std::vector<Sprite*> pSprites)
{
	m_pSprites = pSprites;
}

void FSMComponent::LoadSettings(const std::string& name)
{
	if (name == "Bubble")
		LoadBubbleSettings();
	else if (name == "ZenChan")
		LoadZenChanSettings();
	else if (name == "Player")
		LoadPlayerSettings();
}

Behaviour* FSMComponent::GetBehaviour(const std::string& behaviour) const
{
	for (auto pBehaviour : m_pBehaviours)
	{
		if (pBehaviour->GetName() == behaviour)
			return pBehaviour;
	}

	std::printf("FSMComponent::GetBehaviour() : Behaviour not found\n");
	return nullptr;
}

Sprite* FSMComponent::GetSprite(const std::string& sprite) const
{
	for (auto pSprite : m_pSprites)
	{
		if (pSprite->GetName() == sprite)
			return pSprite;
	}

	std::printf("FSMComponent::GetBehaviour() : Behaviour not found\n");
		return nullptr;
}

void FSMComponent::DrawFSMTab()
{
	const ImVec2 windowSize = ImGui::GetWindowSize();

	Columns(2, " ");
	Separator();

	//Behaviours details
	BeginChild("Behaviours", {windowSize.x / 2.f, windowSize.y }, true);
	Text("Behaviours");
	Separator();
	for (unsigned int i{}; i < m_pBehaviours.size(); i++)
	{
		bool open = TreeNode(m_pBehaviours[i]->GetName().c_str());

		if (open)
		{
			m_pBehaviours[i]->DrawInterface();
			if (Button("Starting State"))
			{
				m_StartingBehaviourIndex = i;
				m_pCurrentBehaviour = m_pBehaviours[i];
			}

			if (m_StartingBehaviourIndex == i)
			{
				SameLine();
				Text("Set");
			}

			TreePop();
		}
	}

	EndChild();
	NextColumn();
	//Behaviours available
	BeginChild("PossibleBehaviours", { windowSize.x / 2.f, windowSize.y / 2.f }, true);
	Text("PossibleBehaviours");
	Separator();
	for (auto behaviour : m_pBehaviours)
	{
		PushID(behaviour);
		Button(behaviour->GetName().c_str());
		HandleDragPossibleBehaviour(behaviour);
		PopID();
	}

	EndChild();
	HandleDropPossibleBehaviour();
	//Sprites available
	BeginChild("PossibleSprites", { windowSize.x / 2.f, windowSize.y / 2.f }, true);
	Text("PossibleSprites");
	Separator();
	const float selectableHeight = 20.f;
	for (auto sprite : m_pSprites)
	{
		PushID(sprite);
		PushItemWidth(windowSize.x / 2.f);
		Button(sprite->GetName().c_str());
		HandleDragSprite(sprite);
		PopID();
	}

	EndChild();
	Columns(1);
}

void FSMComponent::DrawSpriteTab()
{
	Text("Sprites");
	Separator();

	//Add sprites
	static char m_ToBeAddedSpriteName[20]{ "" };
	InputText(" ", m_ToBeAddedSpriteName, IM_ARRAYSIZE(m_ToBeAddedSpriteName));
	SameLine();
	if (Button("AddSprite"))
		m_pSprites.push_back(new Sprite{ m_pGameObject, m_ToBeAddedSpriteName });

	//Draw Sprites
	unsigned int i{};
	for (auto it = m_pSprites.begin(); it != m_pSprites.end();)
	{
		PushID(&(*it));

		//Draw Sprites
		(*it)->DrawInterface();

		if (Button("Delete Sprite"))
			it = m_pSprites.erase(it);
		else
			++it;

		PopID();
		Separator();
		i++;
	}
}

void FSMComponent::HandleDragSprite(Sprite* pSprite)
{
	// Drag this object to change the parent
	PushID(pSprite->GetNameRef());
	if (BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		SetDragDropPayload("Sprite", pSprite->GetNameRef(), pSprite->GetName().size(), ImGuiCond_Once);
		Text(pSprite->GetNameRef()); 
		m_pDraggedSprite = pSprite;
		EndDragDropSource();
	}
	PopID();
}

void FSMComponent::HandleDropPossibleBehaviour()
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SelectedBehaviour"))
		{
			auto temp = GameObjectManager::GetInstance()->GetAndRemoveSelectedBehaviour();
			temp->SetGameObject(m_pGameObject);
			temp->SetFSM(this);
			m_pBehaviours.push_back(temp);
		}

		ImGui::EndDragDropTarget();
	}
}

void FSMComponent::HandleDragPossibleBehaviour(Behaviour* pBehaviour)
{
	// Drag this object to change the parent
	if (BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		SetDragDropPayload("Behaviour", &pBehaviour->GetName(), pBehaviour->GetName().size(), ImGuiCond_Once);
		Text(pBehaviour->GetName().c_str()); 
		m_pDraggedBehaviour = pBehaviour;
		EndDragDropSource();
	}
}

void FSMComponent::LoadPlayerSettings()
{
	auto moveSprite = new Sprite(m_pGameObject, "Move");
	auto bubbleTexture = new TextureComponent(m_pGameObject, "GreenChar.png");
	float width = 16.f;
	float height = 16.f;
	float time = 0.1f;
	float space = 16.f;
	int rows = 1;
	int columns = 8;
	moveSprite->SetAttributes(bubbleTexture, "GreenChar.png", width, height, time, space, rows, columns);
	m_pSprites.push_back(moveSprite);

	auto attackSprite = new Sprite(m_pGameObject, "Attack");
	auto zenBubbleTexture = new TextureComponent(m_pGameObject, "GreenShoot.png");
	attackSprite->SetAttributes(zenBubbleTexture, "GreenShoot.png", width, height, time, space, rows, columns);
	m_pSprites.push_back(attackSprite);

	auto idle = GameObjectManager::GetInstance()->CreateBehaviour("IdleBehaviour");
	idle->SetFSM(this);
	idle->SetGameObject(m_pGameObject);
	m_pBehaviours.push_back(idle);

	auto move = GameObjectManager::GetInstance()->CreateBehaviour("RunBehaviour");
	move->SetFSM(this);
	move->SetGameObject(m_pGameObject);
	m_pBehaviours.push_back(move);

	auto jump = GameObjectManager::GetInstance()->CreateBehaviour("JumpBehaviour");
	jump->SetFSM(this);
	jump->SetGameObject(m_pGameObject);
	m_pBehaviours.push_back(jump);

	auto attack = GameObjectManager::GetInstance()->CreateBehaviour("AttackBehaviour");
	attack->SetFSM(this);
	attack->SetGameObject(m_pGameObject);
	m_pBehaviours.push_back(attack);

	idle->SetTransitionsAndSprites({ move, jump, attack }, { moveSprite });
	move->SetTransitionsAndSprites({ idle, jump, attack }, { moveSprite });
	jump->SetTransitionsAndSprites({ idle, move, attack}, { moveSprite });
	attack->SetTransitionsAndSprites({ }, { attackSprite });

	m_pCurrentBehaviour = idle;
	m_StartingBehaviourIndex = 0;
}

void FSMComponent::LoadBubbleSettings()
{
	auto bubbleSprite = new Sprite(m_pGameObject, "Bubble");
	auto bubbleTexture = new TextureComponent(m_pGameObject, "GreenBubble.png");
	float width = 16.f;
	float height = 16.f;
	float time = 0.1f;
	float space = 16.f;
	int rows = 1;
	int columns = 8;
	bubbleSprite->SetAttributes(bubbleTexture, "GreenBubble.png", width, height, time, space, rows, columns);
	m_pSprites.push_back(bubbleSprite);

	auto zenBubbleSprite = new Sprite(m_pGameObject, "ZenChanBubble");
	auto zenBubbleTexture = new TextureComponent(m_pGameObject, "GreenBubbleZen.png");
	width = 16.f;
	height = 16.f;
	time = 0.1f;
	space = 16.f;
	rows = 1;
	columns = 8;
	zenBubbleSprite->SetAttributes(zenBubbleTexture, "GreenBubbleZen.png", width, height, time, space, rows, columns);
	m_pSprites.push_back(zenBubbleSprite);

	auto bubblePopSprite = new Sprite(m_pGameObject, "BubblePop");
	auto bubblePopTexture = new TextureComponent(m_pGameObject, "BubblePop.png");
	bubblePopSprite->SetAttributes(bubblePopTexture, "BubblePop.png", width, height, time, space, rows, columns);
	m_pSprites.push_back(bubblePopSprite);

	auto shoot = GameObjectManager::GetInstance()->CreateBehaviour("BubbleShootBehaviour");
	shoot->SetFSM(this);
	shoot->SetGameObject(m_pGameObject);
	m_pBehaviours.push_back(shoot);

	auto hit = GameObjectManager::GetInstance()->CreateBehaviour("BubbleHitEnemyBehaviour");
	hit->SetFSM(this);
	hit->SetGameObject(m_pGameObject);
	m_pBehaviours.push_back(hit);

	auto pop = GameObjectManager::GetInstance()->CreateBehaviour("BubblePopBehaviour");
	pop->SetFSM(this);
	pop->SetGameObject(m_pGameObject);
	m_pBehaviours.push_back(pop);

	auto bubbleFloat = GameObjectManager::GetInstance()->CreateBehaviour("BubbleFloatBehaviour");
	bubbleFloat->SetFSM(this);
	bubbleFloat->SetGameObject(m_pGameObject);
	m_pBehaviours.push_back(bubbleFloat);

	shoot->SetTransitionsAndSprites({ bubbleFloat, hit }, { bubbleSprite });
	hit->SetTransitionsAndSprites({ pop }, { zenBubbleSprite, zenBubbleSprite });
	pop->SetTransitionsAndSprites({ }, { bubblePopSprite });
	bubbleFloat->SetTransitionsAndSprites({ pop }, { bubbleSprite });

	m_pCurrentBehaviour = shoot;
	m_StartingBehaviourIndex = 0;
}

void FSMComponent::LoadZenChanSettings()
{
	auto zenSprite = new Sprite(m_pGameObject, "ZenChanMove");
	auto zenTexture = new TextureComponent(m_pGameObject, "Zen-chan.png");
	const float width = 16.f;
	const float height = 16.f;
	const float time = 0.1f;
	const float space = 16.f;
	const int rows = 1;
	const int columns = 8;
	zenSprite->SetAttributes(zenTexture, "Zen-chan.png", width, height, time, space, rows, columns);
	m_pSprites.push_back(zenSprite);

	auto move = GameObjectManager::GetInstance()->CreateBehaviour("EnemyMove");
	move->SetFSM(this);
	move->SetGameObject(m_pGameObject);
	m_pBehaviours.push_back(move);

	auto jump = GameObjectManager::GetInstance()->CreateBehaviour("EnemyJump");
	jump->SetFSM(this);
	jump->SetGameObject(m_pGameObject);
	m_pBehaviours.push_back(jump);
	
	auto scan = GameObjectManager::GetInstance()->CreateBehaviour("EnemyScan");
	scan->SetFSM(this);
	scan->SetGameObject(m_pGameObject);
	m_pBehaviours.push_back(scan);

	move->SetTransitionsAndSprites({ jump, nullptr }, { zenSprite });
	jump->SetTransitionsAndSprites({ move, scan }, { zenSprite });
	scan->SetTransitionsAndSprites({ move, nullptr }, { zenSprite });

	m_pCurrentBehaviour = move;
	m_StartingBehaviourIndex = 0;
}
