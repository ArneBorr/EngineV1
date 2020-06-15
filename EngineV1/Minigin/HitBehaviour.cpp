#include "MiniginPCH.h"
#include "HitBehaviour.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Sprite.h"
#include "imgui.h"
#include "FSMComponent.h"
#include "TransformComponent.h"
#include "Subject.h"

HitBehaviour::HitBehaviour()
	: Behaviour("HitBehaviour")
{
}

void HitBehaviour::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf("HitBehaviour::Initialize() : No rigidbody found!\n");
}

void HitBehaviour::Enter()
{
	m_Timer = 0;
	if (m_pSprite)
	{
		m_pSprite->Reset();
		m_pSprite->Play(true);
	}

	if (m_pRigidbody)
	{
		bool player1 = m_pGameObject->HasTags({ "Player1" });
		if (player1)
			m_pRigidbody->GetSubject()->Notify("Player1Hit", m_pGameObject, nullptr);
		else
			m_pRigidbody->GetSubject()->Notify("Player2Hit", m_pGameObject, nullptr);
	}
}

Behaviour* HitBehaviour::HandleInput()
{
	if (m_Timer > m_MaxDeathTime)
		return m_pIdleTransition;

	return nullptr;
}

void HitBehaviour::Update(float elapsedSec)
{
	m_Timer += elapsedSec;

	if (m_pRigidbody)
		m_pRigidbody->SetLinearVelocity({ 0, 0 });

	if (m_pSprite)
		m_pSprite->Update(elapsedSec);
}

void HitBehaviour::Exit()
{
	//Return to spawn position
	auto pTransform = m_pGameObject->GetTransform();
	if (pTransform)
	{
		const auto loc = pTransform->GetInitialLocation();
		pTransform->SetPosition(loc);
	}
}

void HitBehaviour::DrawInterface()
{
	using namespace ImGui;
	Separator();
	Text("Transitions");
	Separator();

	//Set transition, A lot of repetition because of the way imgui works + strange bug, sury for ugly
	Behaviour* temp;
	if (Button("IdleTransition"))
		m_pIdleTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pIdleTransition = temp;
	PrintTransitionSet(m_pIdleTransition);

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
	InputFloat("Max Death Time", &m_MaxDeathTime);
	Separator();
}

void HitBehaviour::SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode)
{
	pNode->append_attribute(pDoc->allocate_attribute("Name", m_Name.c_str()));
	if (m_pIdleTransition)
		pNode->append_attribute(pDoc->allocate_attribute("IdleTransition", m_pIdleTransition->GetName().c_str()));

	pNode->append_attribute(pDoc->allocate_attribute("MaxDeathTime", FloatToXMLChar(pDoc, m_MaxDeathTime)));

	if (m_pSprite)
		pNode->append_attribute(pDoc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));
}

void HitBehaviour::SetAttributes(rapidxml::xml_node<>* pNode)
{
	auto attribute = pNode->first_attribute("IdleTransition");
	if (attribute != 0)
		m_pIdleTransition = m_pFSM->GetBehaviour(attribute->value());

	m_MaxDeathTime = std::stof(pNode->first_attribute("MaxDeathTime")->value());

	attribute = pNode->first_attribute("Sprite");
	if (attribute != 0)
		m_pSprite = m_pFSM->GetSprite(attribute->value());
}
