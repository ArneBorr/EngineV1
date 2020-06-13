#include "MiniginPCH.h"
#include "EnemyJump.h"
#include "RigidbodyComponent.h"
#include "imgui.h"
#include "Sprite.h"
#include "FSMComponent.h"
#include "Blackboard.h"

EnemyJump::EnemyJump()
	: Behaviour("EnemyJump")
{
}

void EnemyJump::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf("EnemyJump::Initialize() : No rigidbody found!\n");
}

void EnemyJump::Enter()
{
	m_pRigidbody->Jump(m_JumpStrength);
	m_Timer = 0;
}

Behaviour* EnemyJump::HandleInput()
{
	bool isDead{ false };
	m_pFSM->GetBlackboard()->GetData("IsDead", isDead);

	if (isDead)
		return m_pLaunchTransition;

	if (m_Timer > m_Buffer && m_pRigidbody->IsOnGround())
	{
		auto random = rand() % 2;

		if (random == 0)
			return m_pMoveTransition;
		else
			return m_pScanTransition;
	}

	return nullptr;
}

void EnemyJump::Update(float elapsedSec)
{
	m_Timer += elapsedSec;
}

void EnemyJump::DrawInterface()
{
	using namespace ImGui;
	Separator();
	Text("Transitions");
	Separator();

	//Set transition, A lot of repetition because of the way imgui works (Problem with passing behaviour and assigning it in another function but the assigning did not register)
	if (Button("MoveTransition"))
		m_pMoveTransition = nullptr;
	Behaviour* temp = HandleTransitionDrop(this);
	if (temp)
		m_pMoveTransition = temp;
	PrintTransitionSet(m_pMoveTransition);

	if (Button("AttackTransition"))
		m_pScanTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pScanTransition = temp;
	PrintTransitionSet(m_pScanTransition);

	if (Button("LaunchTransition"))
		m_pLaunchTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pLaunchTransition = temp;
	PrintTransitionSet(m_pLaunchTransition);

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

void EnemyJump::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	if (m_pMoveTransition)
		node->append_attribute(doc->allocate_attribute("MoveTransition", m_pMoveTransition->GetName().c_str()));
	if (m_pScanTransition)
		node->append_attribute(doc->allocate_attribute("ScanTransition", m_pScanTransition->GetName().c_str()));
	if (m_pLaunchTransition)
		node->append_attribute(doc->allocate_attribute("LaunchTransition", m_pLaunchTransition->GetName().c_str()));

	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));

	node->append_attribute(doc->allocate_attribute("JumpStrength", FloatToXMLChar(doc, m_JumpStrength)));
}

void EnemyJump::SetAttributes(rapidxml::xml_node<>* node)
{
	auto attribute = node->first_attribute("MoveTransition");
	if (attribute != 0)
		m_pMoveTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("ScanTransition");
	if (attribute != 0)
		m_pScanTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("LaunchTransition");
	if (attribute != 0)
		m_pLaunchTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("Sprite");
	if (attribute != 0)
		m_pSprite = m_pFSM->GetSprite(attribute->value());

	m_JumpStrength = std::stof(node->first_attribute("JumpStrength")->value());
}

void EnemyJump::SetTransitionsAndSprites(const std::vector<Behaviour*>& pTransitions, const std::vector<Sprite*>& pSprites)
{
	if (pTransitions.size() == 3)
	{
		m_pMoveTransition = pTransitions[0];
		m_pScanTransition = pTransitions[1];
		m_pLaunchTransition = pTransitions[2];
	}

	if (pSprites.size() > 0)
		m_pSprite = pSprites[0];
}

