#include "MiniginPCH.h"
#include "RunBehaviour.h"
#include "Sprite.h"
#include "FSMComponent.h"
#include "imgui.h"
#include "MovementComponent.h"
#include "InputManager.h"
#include "RigidbodyComponent.h"
#include "Blackboard.h"

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

Behaviour* RunBehaviour::HandleInput()
{
	auto input = InputManager::GetInstance();
	m_HasMovementInput = false;

	if (input->IsActionPressed("Jump") && abs(m_pRigidbody->GetVelocity().y) - 0 < 0.05f)
		return m_pJumpTransition;

	if (!input->IsActionDown("MoveLeft") && !input->IsActionDown("MoveRight") && abs(m_pRigidbody->GetVelocity().x) < 0.05f)
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
		m_pRigidbody->Move({ m_Speed * elapsesSec * m_SpeedSign, 0 }, { m_MaxSpeed, FLT_MAX });

	//Handle Sprite
	if (m_pRigidbody->GetVelocity().x < 0)
		m_pSprite->Flip(true);
	else
		m_pSprite->Flip(false);

	if (m_pSprite)
		m_pSprite->Update(elapsesSec);
}

void RunBehaviour::Exit()
{
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
	InputFloat("MaxSpeed", &m_MaxSpeed);
	Separator();
}

void RunBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	node->append_attribute(doc->allocate_attribute("NrOfTransitions", IntToXMLChar(doc, 3)));
	if (m_pIdleTransition)
		node->append_attribute(doc->allocate_attribute("Transition", m_pIdleTransition->GetName().c_str()));
	if (m_pJumpTransition)
		node->append_attribute(doc->allocate_attribute("Transition", m_pJumpTransition->GetName().c_str()));
	if (m_pShootTransition)
		node->append_attribute(doc->allocate_attribute("Transition", m_pShootTransition->GetName().c_str()));

	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));

	node->append_attribute(doc->allocate_attribute("Speed", FloatToXMLChar(doc, m_Speed)));
	node->append_attribute(doc->allocate_attribute("MaxSpeed", FloatToXMLChar(doc, m_MaxSpeed)));
}

void RunBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	std::vector<std::string> transitions;
	std::string sprite{ "" };
	GetTransitionsAndSpriteFromAtrribute(transitions, node, sprite);

	unsigned int nrOfSavedTransitions = transitions.size();
	if (nrOfSavedTransitions > 0)
		m_pIdleTransition = m_pFSM->GetBehaviour(transitions[0]);
	if (nrOfSavedTransitions > 1)
		m_pJumpTransition = m_pFSM->GetBehaviour(transitions[1]);
	if (nrOfSavedTransitions > 2)
		m_pShootTransition = m_pFSM->GetBehaviour(transitions[2]);

	if (sprite != "")
		m_pSprite = m_pFSM->GetSprite(sprite);

	m_Speed = std::stof(node->first_attribute("Speed")->value());
	m_MaxSpeed = std::stof(node->first_attribute("MaxSpeed")->value());
}
