#include "MiniginPCH.h"
#include "BubbleHitEnemyBehaviour.h"
#include "Sprite.h"
#include "FSMComponent.h"
#include "Blackboard.h"
#include "RigidbodyComponent.h"
#include "imgui.h"

BubbleHitEnemyBehaviour::BubbleHitEnemyBehaviour()
	: Behaviour("BubbleHitEnemyBehaviour")
{
}

void BubbleHitEnemyBehaviour::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf("BubbleHitEnemyBehaviour::Initialize() : Rigidbody Not Found!");
}

void BubbleHitEnemyBehaviour::Enter()
{
	if (m_pRigidbody)
	{
		m_pRigidbody->MoveHorizontal({ 0, 0 });
	}

	GameObject* pEnemy{ nullptr };
	m_pFSM->GetBlackboard()->GetData("Enemy", pEnemy);

	if (pEnemy)
	{
		if (pEnemy->HasTags({ "ZenChan" }))
			m_IsZenChan = true;
		else
			m_IsZenChan = false;
	}

	if (m_pSprite)
		m_pSprite->Play(true);
	if (m_pOtherEnemySprite)
		m_pOtherEnemySprite->Play(true);
	m_Timer = 0;
}

Behaviour* BubbleHitEnemyBehaviour::HandleInput()
{
	if (m_Timer > m_FloatTime)
		return m_pPopTransition;

	return nullptr;
}

void BubbleHitEnemyBehaviour::Update(float elapsedSec)
{
	m_Timer += elapsedSec;

	if (m_Timer < m_MoveTime && m_pRigidbody)
		m_pRigidbody->MoveVertical(m_Speed);

	if (m_pSprite)
		m_pSprite->Update(elapsedSec);
}

void BubbleHitEnemyBehaviour::Render()
{
	if (m_IsZenChan && m_pSprite)
		m_pSprite->Render();
	else if (m_pOtherEnemySprite)
		m_pOtherEnemySprite->Render();
}

void BubbleHitEnemyBehaviour::OnNotify(const std::string&, GameObject*, GameObject* )
{
}

void BubbleHitEnemyBehaviour::DrawInterface()
{
	using namespace ImGui;
	Separator();
	Text("Transitions");
	Separator();

	//Set transition, A lot of repetition because of the way imgui works
	Behaviour* temp;
	if (Button("PopTransition"))
		m_pPopTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pPopTransition = temp;
	PrintTransitionSet(m_pPopTransition);

	//Sprite
	Separator();
	Text("Sprite");
	Separator();
	PushID(m_pSprite);
	if (m_pSprite)
		Button(m_pSprite->GetName().c_str());
	else
		Button("None");
	auto sprite = HandleSpriteDrop();
	if (sprite)
		m_pSprite = sprite;

	PopID();
	PushID(m_pOtherEnemySprite);
	if (m_pOtherEnemySprite)
		Button(m_pOtherEnemySprite->GetName().c_str());
	else
		Button("None");
	sprite = HandleSpriteDrop();
	if (sprite)
		m_pOtherEnemySprite = sprite;
	Separator();
	PopID();

	//Constants
	Text("Constants");
	Separator();
	PushItemWidth(40.f);
	InputFloat("Speed", &m_Speed);
	InputFloat("Move Time", &m_MoveTime);
	InputFloat("Float Time", &m_FloatTime);
	Separator();
}

void BubbleHitEnemyBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));
}

void BubbleHitEnemyBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	auto attribute = node->first_attribute("Sprite");
	if (attribute != 0)
		m_pSprite = m_pFSM->GetSprite(attribute->value());
}
