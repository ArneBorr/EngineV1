#include "MiniginPCH.h"
#include "JumpBehaviour.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "imgui.h"
#include "Sprite.h"
#include "FSMComponent.h"
#include "InputManager.h"
#include "Blackboard.h"

JumpBehaviour::JumpBehaviour()
	: Behaviour("JumpBehaviour")
{
}

void JumpBehaviour::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf("IdleBehaviour::Initialize() : No rigidbody found!\n");
}

void JumpBehaviour::Enter()
{
	m_pRigidbody->Jump(m_JumpStrength);
}

Behaviour* JumpBehaviour::HandleInput()
{
	if (InputManager::GetInstance()->IsActionPressed("MoveLeft") || InputManager::GetInstance()->IsActionPressed("MoveRight"))
		return m_pRunTransition;

	//Shoot
	bool isShooting = false;
	m_pFSM->GetBlackboard()->GetData("IsShooting", isShooting);
	if (InputManager::GetInstance()->IsActionPressed("Shoot") && !isShooting)
		return m_pShootTransition;


	return m_pIdleTransition;
}

void JumpBehaviour::DrawInterface()
{
	using namespace ImGui;
	Separator();
	Text("Transitions");
	Separator();

	//Set transition, A lot of repetition because of the way imgui works (Problem with passing behaviour and assigning it in another function but the assigning did not register)
	Behaviour* temp;
	if (Button("IdleTransition"))
		m_pIdleTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pIdleTransition = temp;
	PrintTransitionSet(m_pIdleTransition);

	if (Button("RunTransition"))
		m_pRunTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pRunTransition = temp;
	PrintTransitionSet(m_pRunTransition);

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
	InputFloat("Jump Strength", &m_JumpStrength);
	Separator();
}

void JumpBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	node->append_attribute(doc->allocate_attribute("NrOfTransitions", IntToXMLChar(doc, 3)));
	if (m_pIdleTransition)
		node->append_attribute(doc->allocate_attribute("Transition", m_pIdleTransition->GetName().c_str()));
	if (m_pRunTransition)
		node->append_attribute(doc->allocate_attribute("Transition", m_pRunTransition->GetName().c_str()));
	if (m_pShootTransition)
		node->append_attribute(doc->allocate_attribute("Transition", m_pShootTransition->GetName().c_str()));

	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));

	node->append_attribute(doc->allocate_attribute("JumpStrength", FloatToXMLChar(doc, m_JumpStrength)));
}

void JumpBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	std::vector<std::string> transitions;
	std::string sprite{ "" };
	GetTransitionsAndSpriteFromAtrribute(transitions, node, sprite);

	unsigned int nrOfSavedTransitions = transitions.size();
	if (nrOfSavedTransitions > 0)
		m_pIdleTransition = m_pFSM->GetBehaviour(transitions[0]);
	if (nrOfSavedTransitions > 1)
		m_pRunTransition = m_pFSM->GetBehaviour(transitions[1]);
	if (nrOfSavedTransitions > 2)
		m_pShootTransition = m_pFSM->GetBehaviour(transitions[2]);

	if (sprite != "")
		m_pSprite = m_pFSM->GetSprite(sprite);

	m_JumpStrength = std::stof(node->first_attribute("JumpStrength")->value());
}
