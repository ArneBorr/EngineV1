#include "MiniginPCH.h"
#include "EnemyScan.h"
#include "imgui.h"
#include "Sprite.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "BoxColliderComponent.h"
#include "FSMComponent.h"
#include "Blackboard.h"

EnemyScan::EnemyScan()
	:Behaviour("EnemyScan")
{
}

void EnemyScan::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf(" EnemyScan::Initialize() : No Rigidbody Found!");
}

void EnemyScan::Enter()
{
	if (m_pRigidbody)
		m_pRigidbody->SetLinearVelocity({ 0, 0 });

	std::cout << "Scan\n";

	m_IsLeftChecked = false;
	m_IsRightChecked = false;
	m_Timer = 0;
	if (m_pSprite)
		m_pSprite->Play(false);
}

Behaviour* EnemyScan::HandleInput()
{
	bool isDead{ false };
	m_pFSM->GetBlackboard()->GetData("IsDead", isDead);

	if (isDead)
		return m_pLaunchTransition;

	//Raycast
	Vector2f pos{};
	if (m_pRigidbody)
	{
		pos = m_pRigidbody->GetPosition();
	}

	int sign = m_IsLeftChecked ? 1 : -1;
	auto closestFicture = m_pGameObject->GetScene()->RayCast({ pos.x / M_PPM, pos.y / M_PPM }, { pos.x / M_PPM + m_ViewRangePlayer * sign / M_PPM, pos.y / M_PPM });

	GameObject* closesttObject{ nullptr };
	if (closestFicture)
	{
		closesttObject = static_cast<BoxColliderComponent*>(closestFicture->GetUserData())->GetGameObject();
		//Is looking at player
		if (closesttObject && closesttObject->HasTags({ "Player" }))
			return m_pAttackTransition;
	}

	//Has scannes everything
	if (m_IsLeftChecked && m_IsRightChecked)
		return m_pRunTransition;

	return nullptr;
}

void EnemyScan::Update(float elapsedSec)
{
	m_Timer += elapsedSec;
	
	if (!m_IsLeftChecked)
	{
		if (m_pRigidbody)
			m_pRigidbody->MoveHorizontal({ -0.1f, 0 });// Not * elapsedSec since there is no acceleration

		m_pSprite->Flip(true);

		if (m_Timer > m_MaxCheckTime)
		{
			m_pRigidbody->SetLinearVelocity({ 0, 0 });
			m_IsLeftChecked = true;
			m_Timer = 0;
			
			std::cout << "LEft Checked\n";
		}
	}
	else if (!m_IsRightChecked)
	{
		if (m_pRigidbody)
			m_pRigidbody->MoveHorizontal({ 0.1f, 0 });// Not * elapsedSec since there is no acceleration

		m_pSprite->Flip(false);

		if (m_Timer > m_MaxCheckTime)
		{
			m_pRigidbody->SetLinearVelocity({ 0, 0 });
			m_IsRightChecked = true;
			m_Timer = 0;	
		}
	}
}

void EnemyScan::DrawInterface()
{
	using namespace ImGui;
	Separator();
	Text("Transitions");
	Separator();

	//Set transition, A lot of repetition because of the way imgui works
	Behaviour* temp;
	if (Button("RunTransition"))
		m_pRunTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pRunTransition = temp;
	PrintTransitionSet(m_pRunTransition);

	if (Button("LaunchTransition"))
		m_pLaunchTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pLaunchTransition = temp;
	PrintTransitionSet(m_pLaunchTransition);

	//Sprite
	Separator();
	Text("Sprite");
	Separator();
	if (m_pSprite)
		Button(m_pSprite->GetName().c_str());
	else
		Button("None");
	auto sprite = HandleSpriteDrop();
	if (sprite)
		m_pSprite = sprite;
	Separator();

	//Constants
	Text("Constants");
	Separator();
	PushItemWidth(40.f);
	InputFloat("ViewRangePlayer", &m_ViewRangePlayer);
	Separator();
}

void EnemyScan::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	if (m_pRunTransition)
		node->append_attribute(doc->allocate_attribute("RunTransition", m_pRunTransition->GetName().c_str()));

	if (m_pAttackTransition)
		node->append_attribute(doc->allocate_attribute("AttackTransition", m_pAttackTransition->GetName().c_str()));

	if (m_pLaunchTransition)
		node->append_attribute(doc->allocate_attribute("LaunchTransition", m_pLaunchTransition->GetName().c_str()));

	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));

	node->append_attribute(doc->allocate_attribute("MaxCheckTime", FloatToXMLChar(doc, m_MaxCheckTime)));
	node->append_attribute(doc->allocate_attribute("ViewRangePlayer", FloatToXMLChar(doc, m_ViewRangePlayer)));
}

void EnemyScan::SetAttributes(rapidxml::xml_node<>* node)
{
	auto attribute = node->first_attribute("RunTransition");
	if (attribute != 0)
		m_pRunTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("AttackTransition");
	if (attribute != 0)
		m_pAttackTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("LaunchTransition");
	if (attribute != 0)
		m_pLaunchTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("Sprite");
	if (attribute != 0)
		m_pSprite = m_pFSM->GetSprite(attribute->value());

	m_MaxCheckTime = std::stof(node->first_attribute("MaxCheckTime")->value());
	m_ViewRangePlayer = std::stof(node->first_attribute("ViewRangePlayer")->value());
}

void EnemyScan::SetTransitionsAndSprites(const std::vector<Behaviour*>& pTransitions, const std::vector<Sprite*>& pSprites)
{
	if (pTransitions.size() == 3)
	{
		m_pRunTransition = pTransitions[0];
		m_pAttackTransition = pTransitions[1];
		m_pLaunchTransition = pTransitions[2];
	}

	if (pSprites.size() > 0)
		m_pSprite = pSprites[0];
}
