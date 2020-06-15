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
	for (auto pSprite : m_pSprites)
	{
		delete pSprite;
		pSprite = nullptr;
	}
	m_pSprites.clear();

	for (auto pBehaviour : m_pBehaviours)
	{
		delete pBehaviour;
		pBehaviour = nullptr;
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
		for (auto pBehaviour : m_pBehaviours)
			pBehaviour->Initialize();

		m_pCurrentBehaviour->Enter();
	}
}

void FSMComponent::Render()
{
	if (m_IsFSMPaused)
		return;

	//Only Render Behaviour on top
	if (m_pOnTopBehaviour)
		m_pOnTopBehaviour->Render();
	else if (m_pCurrentBehaviour)
		m_pCurrentBehaviour->Render();
}

void FSMComponent::Update(float elapsedSec)
{
	if (!GameInfo::GetInstance()->IsPlaying() || m_IsFSMPaused)
		return;

	Behaviour* pBehaviour = m_pCurrentBehaviour->HandleInput();
	//If state wants to change
	if (pBehaviour && pBehaviour != m_pCurrentBehaviour)
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

void FSMComponent::SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode)
{
	pNode->append_attribute(pDoc->allocate_attribute("StartingState", IntToXMLChar(pDoc, int(m_StartingBehaviourIndex))));
	//Sprites
	rapidxml::xml_node<>* pMainSpritesNode = pDoc->allocate_node(rapidxml::node_element, "Sprites");
	for (auto sprite : m_pSprites)
	{
		rapidxml::xml_node<>* spriteNode = pDoc->allocate_node(rapidxml::node_element, "Sprite");
		sprite->SaveAttributes(pDoc, spriteNode);
		pMainSpritesNode->append_node(spriteNode);
	}
	pNode->append_node(pMainSpritesNode);

	//Behaviours
	rapidxml::xml_node<>* pMainBehavioursNode = pDoc->allocate_node(rapidxml::node_element, "Behaviours");
	for (auto behaviour : m_pBehaviours)
	{
		rapidxml::xml_node<>* pBehaviourNode = pDoc->allocate_node(rapidxml::node_element, "Behaviour");
		behaviour->SaveAttributes(pDoc, pBehaviourNode);
		pMainBehavioursNode->append_node(pBehaviourNode);
	}
	pNode->append_node(pMainBehavioursNode);
}

void FSMComponent::SetAttributes(rapidxml::xml_node<>* pNode)
{
	int i{};
	for (rapidxml::xml_node<>* behaviourNode = pNode->first_node(); behaviourNode != 0; behaviourNode = behaviourNode->next_sibling())
	{
		m_pBehaviours[i]->SetAttributes(behaviourNode);
		++i;
	}

	if (m_pBehaviours.size() > 0)
		m_pCurrentBehaviour = m_pBehaviours[m_StartingBehaviourIndex];
}

void FSMComponent::OnNotify(const std::string& event, GameObject* pObject, GameObject* pObjCollWith)
{
	if (m_IsFSMPaused)
		return;

	if (m_pOnTopBehaviour)
		m_pOnTopBehaviour->OnNotify(event, pObject, pObjCollWith);

	if (m_pCurrentBehaviour)
		m_pCurrentBehaviour->OnNotify(event, pObject, pObjCollWith);
}

void FSMComponent::SetBehaviours(const std::vector<Behaviour*>& pBehaviours)
{
	m_pBehaviours = pBehaviours;
}

void FSMComponent::SetSprites(const std::vector<Sprite*> pSprites)
{
	m_pSprites = pSprites;
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
			//Draw paramters of behaviour
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

	//Show Behaviours available
	BeginChild("PossibleBehaviours", { windowSize.x / 2.f, windowSize.y / 2.f }, true);
	Text("PossibleBehaviours");
	Separator();
	for (auto pBehaviours : m_pBehaviours)
	{
		PushID(pBehaviours);
		Button(pBehaviours->GetName().c_str());
		HandleDragPossibleBehaviour(pBehaviours);
		PopID();
	}
	EndChild();
	HandleDropPossibleBehaviour();

	//Show Sprites available
	BeginChild("PossibleSprites", { windowSize.x / 2.f, windowSize.y / 2.f }, true);
	Text("PossibleSprites");
	Separator();
	const float selectableHeight = 20.f;
	for (auto pSprite : m_pSprites)
	{
		PushID(pSprite);
		PushItemWidth(windowSize.x / 2.f);
		Button(pSprite->GetName().c_str());
		HandleDragSprite(pSprite);
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
		if (const ImGuiPayload* pPayload = ImGui::AcceptDragDropPayload("SelectedBehaviour"))
		{
			auto pBehaviour = GameObjectManager::GetInstance()->GetAndRemoveSelectedBehaviour();
			pBehaviour->SetGameObject(m_pGameObject);
			pBehaviour->SetFSM(this);
			m_pBehaviours.push_back(pBehaviour);
		}

		ImGui::EndDragDropTarget();
	}
}

void FSMComponent::HandleDragPossibleBehaviour(Behaviour* pBehaviour)
{
	if (BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		SetDragDropPayload("Behaviour", &pBehaviour->GetName(), pBehaviour->GetName().size(), ImGuiCond_Once);
		Text(pBehaviour->GetName().c_str()); 
		m_pDraggedBehaviour = pBehaviour;
		EndDragDropSource();
	}
}
