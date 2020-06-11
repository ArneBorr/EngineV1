#include "MiniginPCH.h"
#include "IdleBehaviour.h"
#include "imgui.h"
#include "Sprite.h"
#include "FSMComponent.h"

IdleBehaviour::IdleBehaviour()
	: Behaviour( "IdleBehaviour" )
{
}

void IdleBehaviour::Enter()
{
}

Behaviour* IdleBehaviour::HandleInput()
{
	return nullptr;
}

void IdleBehaviour::Update(float elapsesSec)
{
	UNREFERENCED_PARAMETER(elapsesSec);
}

void IdleBehaviour::Exit()
{
}

void IdleBehaviour::DrawInterface()
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
}

void IdleBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	node->append_attribute(doc->allocate_attribute("NrOfTransitions", IntToXMLChar(doc, 3)));
	if (m_pRunTransition)
		node->append_attribute(doc->allocate_attribute("Transition", m_pRunTransition->GetName().c_str()));
	if (m_pJumpTransition)
		node->append_attribute(doc->allocate_attribute("Transition", m_pJumpTransition->GetName().c_str()));
	if (m_pShootTransition)
		node->append_attribute(doc->allocate_attribute("Transition", m_pShootTransition->GetName().c_str()));
	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));
}

void IdleBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	std::vector<std::string> transitions;
	std::string sprite{""};
	GetTransitionsAndSpriteFromAtrribute(transitions, node, sprite);

	unsigned int nrOfSavedTransitions =transitions.size();
	if (nrOfSavedTransitions > 0)
		m_pRunTransition = m_pFSM->GetBehaviour(transitions[0]);
	if (nrOfSavedTransitions > 1)
		m_pJumpTransition = m_pFSM->GetBehaviour(transitions[1]);
	if (nrOfSavedTransitions > 2)
		m_pShootTransition = m_pFSM->GetBehaviour(transitions[2]);

	if (sprite != "")
		m_pSprite = m_pFSM->GetSprite(sprite);
}