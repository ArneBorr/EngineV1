#include "MiniginPCH.h"
#include "IdleBehaviour.h"
#include "imgui.h"
#include "Sprite.h"
#include "FSMComponent.h"
#include "InputManager.h"
#include "RigidbodyComponent.h"
#include "Blackboard.h"
#include "Subject.h"

IdleBehaviour::IdleBehaviour()
	: Behaviour( "IdleBehaviour" )
{
}

void IdleBehaviour::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf("IdleBehaviour::Initialize() : No rigidbody found!\n");
}

void IdleBehaviour::Enter()
{
	if (m_pSprite)
	{
		m_pSprite->Reset();
		m_pSprite->Play(false);
	}

	if (m_pRigidbody)
		m_pRigidbody->GetSubject()->AddObserver(this);
	m_IsHit = false;
}

Behaviour* IdleBehaviour::HandleInput()
{
	if (m_IsHit)
		return m_pHitTransition;

	//Determine if p1/p2
	PlayerAction player = m_pGameObject->HasTags({ "Player2" }) ? PlayerAction::Two : PlayerAction::One;

	//Jump
	if (m_pRigidbody)
	{
		if (InputManager::GetInstance()->IsActionPressed("Jump", player) && abs(m_pRigidbody->GetVelocity().y) - 0 < 0.05f && m_pRigidbody->IsOnGround())
			return m_pJumpTransition;	
	}
	
	//Shoot
	bool isShooting = false;
	m_pFSM->GetBlackboard()->GetData("IsShooting", isShooting);
	if (InputManager::GetInstance()->IsActionPressed("Shoot", player))
		return m_pShootTransition;

	//Move
	if (InputManager::GetInstance()->IsActionDown("MoveLeft", player) || InputManager::GetInstance()->IsActionDown("MoveRight", player))
		return m_pRunTransition;

	return nullptr;
}

void IdleBehaviour::OnNotify(const std::string& event, GameObject* pObject, GameObject*)
{
	if ((event == "ProjectileEntered" || event == "EnemyEntered") && pObject == m_pGameObject)
		m_IsHit = true;
}

void IdleBehaviour::Exit()
{
	if (m_pRigidbody)
		m_pRigidbody->GetSubject()->RemoveObserver(this);
}

void IdleBehaviour::DrawInterface()
{
	using namespace ImGui;
	Separator();
	Text("Transitions");
	Separator();

	//Set transition, A lot of repetition because of the way imgui works + strange bug, sury for ugly
	Behaviour* temp;
	//Delete transition when button clicked
	if (Button("RunTransition"))
		m_pRunTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pRunTransition = temp;
	PrintTransitionSet(m_pRunTransition);

	if (Button("JumpTransition"))
		m_pJumpTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pJumpTransition = temp;
	PrintTransitionSet(m_pJumpTransition);

	if (Button("ShootTransition"))
		m_pShootTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pShootTransition = temp;
	PrintTransitionSet(m_pShootTransition);

	if (Button("HitTransition"))
		m_pHitTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pHitTransition = temp;
	PrintTransitionSet(m_pHitTransition);

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
}

void IdleBehaviour::SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode)
{
	pNode->append_attribute(pDoc->allocate_attribute("Name", m_Name.c_str()));
	if (m_pRunTransition)
		pNode->append_attribute(pDoc->allocate_attribute("RunTransition", m_pRunTransition->GetName().c_str()));
	if (m_pJumpTransition)
		pNode->append_attribute(pDoc->allocate_attribute("JumpTransition", m_pJumpTransition->GetName().c_str()));
	if (m_pShootTransition)
		pNode->append_attribute(pDoc->allocate_attribute("ShootTransition", m_pShootTransition->GetName().c_str()));
	if (m_pShootTransition)
		pNode->append_attribute(pDoc->allocate_attribute("HitTransition", m_pHitTransition->GetName().c_str()));

	if (m_pSprite)
		pNode->append_attribute(pDoc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));
}

void IdleBehaviour::SetAttributes(rapidxml::xml_node<>* pNode)
{
	auto attribute = pNode->first_attribute("RunTransition");
	if (attribute != 0)
		m_pRunTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = pNode->first_attribute("JumpTransition");
	if (attribute != 0)
		m_pJumpTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = pNode->first_attribute("ShootTransition");
	if (attribute != 0)
		m_pShootTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = pNode->first_attribute("HitTransition");
	if (attribute != 0)
		m_pHitTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = pNode->first_attribute("Sprite");
	if (attribute != 0)
		m_pSprite = m_pFSM->GetSprite(attribute->value());
}
