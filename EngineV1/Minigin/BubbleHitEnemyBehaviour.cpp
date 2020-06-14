#include "MiniginPCH.h"
#include "BubbleHitEnemyBehaviour.h"
#include "Sprite.h"
#include "FSMComponent.h"
#include "Blackboard.h"
#include "RigidbodyComponent.h"
#include "imgui.h"
#include "TransformComponent.h"
#include "Subject.h"

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
		m_pRigidbody->GetSubject()->AddObserver(this);
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

	if (m_IsZenChan && m_pSprite)
		m_pSpriteToRender = m_pSprite;
	else if (!m_IsZenChan && m_pMaitaSprite)
		m_pSpriteToRender = m_pMaitaSprite;

	if (m_pSpriteToRender)
		m_pSpriteToRender->Play(true);

	m_Timer = 0;
	m_IsCaught = false;
}

Behaviour* BubbleHitEnemyBehaviour::HandleInput()
{
	if (m_Timer > m_FloatTime || m_IsCaught)
		return m_pPopTransition;

	return nullptr;
}

void BubbleHitEnemyBehaviour::Update(float elapsedSec)
{
	m_Timer += elapsedSec;

	if (m_pRigidbody)
	{
		if (m_Timer < m_MoveTime)
			m_pRigidbody->MoveVertical(-m_Speed); // Not * elapsedSec since it is a constant velocity
		else
			m_pRigidbody->MoveVertical(0);
	}

	if (m_pSpriteToRender)
		m_pSpriteToRender->Update(elapsedSec);
}

void BubbleHitEnemyBehaviour::Exit()
{
	//Release Enemy
	GameObject* pEnemy{ nullptr };
	m_pFSM->GetBlackboard()->GetData("Enemy", pEnemy);
	if (pEnemy)
	{
		auto pFSM = pEnemy->GetComponent<FSMComponent>();
		if (pFSM)
			pFSM->Pause(false);

		auto pTransform = pEnemy->GetRigidbody();
		if (pTransform && m_pRigidbody)
		{
			pTransform->SetPosition(m_pRigidbody->GetPosition());
			m_pRigidbody->GetSubject()->RemoveObserver(this);
		}
	}
}

void BubbleHitEnemyBehaviour::Render()
{
	if (m_pSpriteToRender)
		m_pSpriteToRender->Render();
}

void BubbleHitEnemyBehaviour::OnNotify(const std::string& event, GameObject* pObject , GameObject*)
{
	if (event == "PlayerEntered")
	{
		if (pObject == m_pFSM->GetGameObject())
		{
			GameObject* pEnemy{ nullptr };
			m_pFSM->GetBlackboard()->GetData("Enemy", pEnemy);
			if (pEnemy)
			{
				auto pFSM = pEnemy->GetComponent<FSMComponent>();
				if (pFSM)
				{
					pFSM->GetBlackboard()->SetData("IsDead", true);
					pFSM->Pause(false);
					m_IsCaught = true;
				}
			}	
		}
	}
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
	PushID(m_pMaitaSprite);
	if (m_pMaitaSprite)
		Button(m_pMaitaSprite->GetName().c_str());
	else
		Button("None");
	sprite = HandleSpriteDrop();
	if (sprite)
		m_pMaitaSprite = sprite;
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
	if (m_pPopTransition)
		node->append_attribute(doc->allocate_attribute("PopTransition", m_pPopTransition->GetName().c_str()));
	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("SpriteZenChan", m_pSprite->GetNameRef()));
	if (m_pMaitaSprite)
		node->append_attribute(doc->allocate_attribute("SpriteMaita ", m_pMaitaSprite->GetNameRef()));

	node->append_attribute(doc->allocate_attribute("Speed", FloatToXMLChar(doc, m_Speed)));
	node->append_attribute(doc->allocate_attribute("MoveTime", FloatToXMLChar(doc, m_MoveTime)));
	node->append_attribute(doc->allocate_attribute("FloatTime", FloatToXMLChar(doc, m_FloatTime)));
}

void BubbleHitEnemyBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	auto attribute = node->first_attribute("PopTransition");
	if (attribute != 0)
		m_pPopTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("SpriteZenChan");
	if (attribute != 0)
		m_pSprite = m_pFSM->GetSprite(attribute->value());

	attribute = node->first_attribute("SpriteMaita");
	if (attribute != 0)
		m_pMaitaSprite = m_pFSM->GetSprite(attribute->value());

	m_Speed = std::stof(node->first_attribute("Speed")->value());
	m_MoveTime = std::stof(node->first_attribute("MoveTime")->value());
	m_FloatTime = std::stof(node->first_attribute("FloatTime")->value());
}