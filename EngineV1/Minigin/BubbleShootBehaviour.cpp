#include "MiniginPCH.h"
#include "BubbleShootBehaviour.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Sprite.h"
#include "imgui.h"
#include "FSMComponent.h"

BubbleShootBehaviour::BubbleShootBehaviour()
	: Behaviour("BubbleShootBehaviour")
{
}

void BubbleShootBehaviour::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf("BubbleShootBehaviour::Initialize() : No Rigidbody Found! \n");
}

void BubbleShootBehaviour::Enter()
{
	if (m_pRigidbody)
	{
		if (m_pRigidbody->GetVelocity().x < 0)
			m_Speed = -m_Speed;
	}

	m_Timer = 0;

	if(m_pSprite)
		m_pSprite->Play(true);
}

Behaviour* BubbleShootBehaviour::HandleInput()
{
	if (m_Timer > m_ShootTime)
		return m_pBubbleFloat;

	return nullptr;
}

void BubbleShootBehaviour::Update(float elapsedSec)
{
	m_Timer += elapsedSec;

	if (m_pRigidbody)
	{
		m_pRigidbody->Move({ m_Speed * elapsedSec, 0 }, { m_MaxSpeed, 0 });
	}

	if (m_pSprite)
		m_pSprite->Update(elapsedSec);
}

void BubbleShootBehaviour::Exit()
{
	if (m_pRigidbody)
		m_pRigidbody->SetVelocity({ 0, 0 });
}

void BubbleShootBehaviour::DrawInterface()
{
	using namespace ImGui;
	Separator();
	Text("Transitions");
	Separator();

	//Set transition, A lot of repetition because of the way imgui works
	Behaviour* temp;
	if (Button("FloatTransition"))
		m_pBubbleFloat = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pBubbleFloat = temp;
	PrintTransitionSet(m_pBubbleFloat);

	if (Button("HitTransition"))
		m_pBubbleHit = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pBubbleHit = temp;
	PrintTransitionSet(m_pBubbleHit);

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
	InputFloat("Max Speed", &m_MaxSpeed);
	InputFloat("Shoot Time", &m_ShootTime);
	Separator();
}

void BubbleShootBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	node->append_attribute(doc->allocate_attribute("NrOfTransitions", IntToXMLChar(doc, 2)));
	if (m_pBubbleFloat)
		node->append_attribute(doc->allocate_attribute("Transition", m_pBubbleFloat->GetName().c_str()));
	if (m_pBubbleHit)
		node->append_attribute(doc->allocate_attribute("Transition", m_pBubbleHit->GetName().c_str()));

	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));

	node->append_attribute(doc->allocate_attribute("Speed", FloatToXMLChar(doc, m_Speed)));
	node->append_attribute(doc->allocate_attribute("MaxSpeed", FloatToXMLChar(doc, m_MaxSpeed)));
	node->append_attribute(doc->allocate_attribute("ShootTime", FloatToXMLChar(doc, m_ShootTime)));
}

void BubbleShootBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	std::vector<std::string> transitions;
	std::string sprite{ "" };
	GetTransitionsAndSpriteFromAtrribute(transitions, node, sprite);

	for (unsigned int i{}; i < transitions.size(); i++)
	{
		if (transitions[i] == "BubbleFloatBehaviour")
			m_pBubbleFloat = m_pFSM->GetBehaviour(transitions[i]);
		else if (transitions[i] == "BubbleHitEnemyBehaviour")
			m_pBubbleHit = m_pFSM->GetBehaviour(transitions[i]);
	}

	if (sprite != "")
		m_pSprite = m_pFSM->GetSprite(sprite);

	m_Speed = std::stof(node->first_attribute("Speed")->value());
	m_MaxSpeed = std::stof(node->first_attribute("MaxSpeed")->value());
	m_ShootTime = std::stof(node->first_attribute("ShootTime")->value());
}

void BubbleShootBehaviour::SetTransitionsAndSprite(const std::vector<Behaviour*>& pTransitions, Sprite* pSprite)
{
	if (pTransitions.size() == 2)
	{
		m_pBubbleFloat = pTransitions[0];
		m_pBubbleHit = pTransitions[1];
	}

	m_pSprite = pSprite;
}
