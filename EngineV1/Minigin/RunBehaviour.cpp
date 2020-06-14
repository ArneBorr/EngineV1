#include "MiniginPCH.h"
#include "RunBehaviour.h"
#include "Sprite.h"
#include "FSMComponent.h"
#include "imgui.h"
#include "MovementComponent.h"
#include "InputManager.h"
#include "RigidbodyComponent.h"
#include "Blackboard.h"
#include "Subject.h"

RunBehaviour::RunBehaviour()
	: Behaviour("RunBehaviour")
{
}

void RunBehaviour::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf("IdleBehaviour::Initialize() : No rigidbody found!\n");

	m_pFSM->GetBlackboard()->AddData("IsFacingLeft", false);
}

void RunBehaviour::Enter()
{
	if (m_pRigidbody)
		m_pRigidbody->GetSubject()->AddObserver(this);

	m_pSprite->Play(true);
	m_IsHit = false;
}

Behaviour* RunBehaviour::HandleInput()
{
	if (m_IsHit)
		return m_pHitTransition;

	auto input = InputManager::GetInstance();
	m_HasMovementInput = false;

	if (input->IsActionPressed("Jump") && abs(m_pRigidbody->GetVelocity().y) - 0 < 0.05f && m_pRigidbody->IsOnGround())
		return m_pJumpTransition;

	if (!input->IsActionDown("MoveLeft") && !input->IsActionDown("MoveRight") && abs(m_pRigidbody->GetVelocity().x) < 0.05f )
		return m_pIdleTransition;

	//Shoot
	bool isShooting = false;
	m_pFSM->GetBlackboard()->GetData("IsShooting", isShooting);
	if (InputManager::GetInstance()->IsActionPressed("Shoot") && !isShooting)
		return m_pShootTransition;

	if (input->IsActionDown("MoveLeft"))
	{
		m_HasMovementInput = true;
		m_SpeedSign = -1;
		m_pFSM->GetBlackboard()->SetData("IsFacingLeft", true);
	}
	else if (input->IsActionDown("MoveRight"))
	{
		m_HasMovementInput = true;
		m_SpeedSign = 1;
		m_pFSM->GetBlackboard()->SetData("IsFacingLeft", false);
	}

	return nullptr;
}

void RunBehaviour::Update(float elapsesSec)
{
	//Move
	if (m_HasMovementInput)
		m_pRigidbody->MoveHorizontal({ m_Speed * m_SpeedSign, 0 }); // Not * elapsedSec since there is no acceleration

	//Handle Sprite
	if (m_pRigidbody->GetVelocity().x < 0)
		m_pSprite->Flip(true);
	else
		m_pSprite->Flip(false);

	if (m_pSprite)
		m_pSprite->Update(elapsesSec);
}

void RunBehaviour::OnNotify(const std::string& event, GameObject* pObject, GameObject*)
{
	if ((event == "ProjectileEntered" || event == "EnemyEntered") && pObject == m_pGameObject)
		m_IsHit = true;
}

void RunBehaviour::Exit()
{
	if (m_pRigidbody)
		m_pRigidbody->GetSubject()->RemoveObserver(this);
}

void RunBehaviour::DrawInterface()
{
	using namespace ImGui;
	Separator();
	Text("Transitions");
	Separator();

	//Set transition, A lot of repetition because of the way imgui works
	Behaviour* temp;
	if (Button("IdleTransition"))
		m_pIdleTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pIdleTransition = temp;
	PrintTransitionSet(m_pIdleTransition);

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

	//Constants
	Text("Constants");
	Separator();
	PushItemWidth(40.f);
	InputFloat("Speed", &m_Speed);
	Separator();
}

void RunBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	if (m_pIdleTransition)
		node->append_attribute(doc->allocate_attribute("IdleTransition", m_pIdleTransition->GetName().c_str()));
	if (m_pJumpTransition)
		node->append_attribute(doc->allocate_attribute("JumpTransition", m_pJumpTransition->GetName().c_str()));
	if (m_pShootTransition)
		node->append_attribute(doc->allocate_attribute("ShootTransition", m_pShootTransition->GetName().c_str()));
	if (m_pHitTransition)
		node->append_attribute(doc->allocate_attribute("HitTransition", m_pHitTransition->GetName().c_str()));

	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));

	node->append_attribute(doc->allocate_attribute("Speed", FloatToXMLChar(doc, m_Speed)));
}

void RunBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	auto attribute = node->first_attribute("IdleTransition");
	if (attribute != 0)
		m_pIdleTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("JumpTransition");
	if (attribute != 0)
		m_pJumpTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("ShootTransition");
	if (attribute != 0)
		m_pShootTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("HitTransition");
	if (attribute != 0)
		m_pHitTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("Sprite");
	if (attribute != 0)
		m_pSprite = m_pFSM->GetSprite(attribute->value());

	m_Speed = std::stof(node->first_attribute("Speed")->value());
}
