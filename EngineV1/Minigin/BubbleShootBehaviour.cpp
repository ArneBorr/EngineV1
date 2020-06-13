#include "MiniginPCH.h"
#include "BubbleShootBehaviour.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Sprite.h"
#include "imgui.h"
#include "FSMComponent.h"
#include "Blackboard.h"

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
	bool isFacingLeft = false;
	m_pFSM->GetBlackboard()->GetData("IsFacingLeft", isFacingLeft);

	if (isFacingLeft)
		m_Speed *= -1;

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
		m_pRigidbody->MoveHorizontal({ m_Speed, 0 }); // Not * elapsedSec since there is no acceleration
	}

	if (m_pSprite)
		m_pSprite->Update(elapsedSec);
}

void BubbleShootBehaviour::Exit()
{
	if (m_pRigidbody)
		m_pRigidbody->SetVelocity({ 0, 0 });
}

void BubbleShootBehaviour::OnNotify(const std::string& event, GameObject* pObject)
{
	if (event == "EnemyEntered")
	{
		auto fsm = pObject->GetComponent<FSMComponent>();
		if (fsm)
			fsm->Pause(true);

		std::cout << "Registered\n";
		if (!m_pFSM->GetBlackboard()->AddData("Enemy", pObject))
			m_pFSM->GetBlackboard()->SetData("Enemy", pObject);
	}
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
	InputFloat("Shoot Time", &m_ShootTime);
	Separator();
}

void BubbleShootBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));

	if (m_pBubbleFloat)
		node->append_attribute(doc->allocate_attribute("FloatTransition", m_pBubbleFloat->GetName().c_str()));
	if (m_pBubbleHit)
		node->append_attribute(doc->allocate_attribute("HitTransition", m_pBubbleHit->GetName().c_str()));

	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));

	node->append_attribute(doc->allocate_attribute("Speed", FloatToXMLChar(doc, m_Speed)));
	node->append_attribute(doc->allocate_attribute("ShootTime", FloatToXMLChar(doc, m_ShootTime)));
}

void BubbleShootBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	auto attribute = node->first_attribute("FloatTransition");
	if (attribute != 0)
		m_pBubbleFloat = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("HitTransition");
	if (attribute != 0)
		m_pBubbleHit = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("Sprite");
	if (attribute != 0)
		m_pSprite = m_pFSM->GetSprite(attribute->value());

	m_Speed = std::stof(node->first_attribute("Speed")->value());
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
