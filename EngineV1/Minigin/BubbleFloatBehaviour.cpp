#include "MiniginPCH.h"
#include "BubbleFloatBehaviour.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Sprite.h"
#include "imgui.h"
#include "FSMComponent.h"

BubbleFloatBehaviour::BubbleFloatBehaviour()
	: Behaviour("BubbleFloatBehaviour")
{
}

void BubbleFloatBehaviour::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf("BubbleFloatBehaviour::Initialize() : Rigidbody Not Found!\n");
}

void BubbleFloatBehaviour::Enter()
{
	m_Timer = 0;
	if (m_pSprite)
		m_pSprite->Play(true);
}

Behaviour* BubbleFloatBehaviour::HandleInput()
{
	if (m_Timer > m_FloatTime)
		return m_pBubblePop;

	return nullptr;
}

void BubbleFloatBehaviour::Update(float elapsedSec)
{
	m_Timer += elapsedSec;

	if (m_Timer < m_MoveTime)
		m_pRigidbody->MoveHorizontal({ 0, -m_Speed }); // Not * elapsedSec since there is no acceleration
	else
		m_pRigidbody->SetVelocity({ 0, 0 });

	if (m_pSprite)
		m_pSprite->Update(elapsedSec);
}

void BubbleFloatBehaviour::Exit()
{
	if (m_pRigidbody)
		m_pRigidbody->SetVelocity({ 0, 0 });
}

void BubbleFloatBehaviour::DrawInterface()
{
	using namespace ImGui;
	Separator();
	Text("Transitions");
	Separator();

	//Set transition, A lot of repetition because of the way imgui works
	Behaviour* temp;
	if (Button("PopTransition"))
		m_pBubblePop = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pBubblePop = temp;
	PrintTransitionSet(m_pBubblePop);

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
	InputFloat("Move Time", &m_MoveTime);
	InputFloat("Float Time", &m_FloatTime);
	Separator();
}

void BubbleFloatBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	node->append_attribute(doc->allocate_attribute("NrOfTransitions", IntToXMLChar(doc, 1)));
	if (m_pBubblePop)
		node->append_attribute(doc->allocate_attribute("Transition", m_pBubblePop->GetName().c_str()));

	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));

	node->append_attribute(doc->allocate_attribute("Speed", FloatToXMLChar(doc, m_Speed)));
	node->append_attribute(doc->allocate_attribute("MoveTime", FloatToXMLChar(doc, m_MoveTime)));
	node->append_attribute(doc->allocate_attribute("FloatTime", FloatToXMLChar(doc, m_FloatTime)));
}

void BubbleFloatBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	std::vector<std::string> transitions;
	std::string sprite{ "" };
	GetTransitionsAndSpriteFromAtrribute(transitions, node, sprite);

	for (unsigned int i{}; i < transitions.size(); i++)
	{
		if (transitions[i] == "BubblePopBehaviour")
			m_pBubblePop = m_pFSM->GetBehaviour(transitions[i]);
	}

	if (sprite != "")
		m_pSprite = m_pFSM->GetSprite(sprite);

	m_Speed = std::stof(node->first_attribute("Speed")->value());
	m_MoveTime = std::stof(node->first_attribute("MoveTime")->value());
	m_FloatTime = std::stof(node->first_attribute("FloatTime")->value());
}

void BubbleFloatBehaviour::SetTransitionsAndSprite(const std::vector<Behaviour*>& pTransitions, Sprite* pSprite)
{
	if (pTransitions.size() == 1)
		m_pBubblePop = pTransitions[0];

	m_pSprite = pSprite;
}
