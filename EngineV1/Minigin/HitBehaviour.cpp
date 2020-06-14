#include "MiniginPCH.h"
#include "HitBehaviour.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Sprite.h"
#include "imgui.h"
#include "FSMComponent.h"
#include "TransformComponent.h"

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

	//Set transition, A lot of repetition because of the way imgui works
	Behaviour* temp;
	if (Button("IdleTransition"))
		m_pIdleTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pIdleTransition = temp;
	PrintTransitionSet(m_pIdleTransition);

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

void HitBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	if (m_pIdleTransition)
		node->append_attribute(doc->allocate_attribute("IdleTransition", m_pIdleTransition->GetName().c_str()));

	node->append_attribute(doc->allocate_attribute("MaxDeathTime", FloatToXMLChar(doc, m_MaxDeathTime)));

	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));
}

void HitBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	auto attribute = node->first_attribute("IdleTransition");
	if (attribute != 0)
		m_pIdleTransition = m_pFSM->GetBehaviour(attribute->value());

	m_MaxDeathTime = std::stof(node->first_attribute("MaxDeathTime")->value());

	attribute = node->first_attribute("Sprite");
	if (attribute != 0)
		m_pSprite = m_pFSM->GetSprite(attribute->value());
}
