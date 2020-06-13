#include "MiniginPCH.h"
#include "EnemyMove.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Sprite.h"
#include "Scene.h"
#include "BoxColliderComponent.h"
#include "imgui.h"
#include "FSMComponent.h"
#include "Blackboard.h"

EnemyMove::EnemyMove()
	: Behaviour{ "EnemyMove" }
{
}

void EnemyMove::Initialize()
{
	m_pFSM->GetBlackboard()->AddData("IsDead", false);
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf("IdleBehaviour::Initialize() : No rigidbody found!\n");
}

void EnemyMove::Enter()
{
	m_Timer = 0;
	if (m_pSprite)
	{
		m_pSprite->Reset();
		m_pSprite->Play(true);
	}
	
	m_HasMovedEnough = false;
	m_CurrentMaxTimer = std::rand() % int((m_MaxTimer - m_MinTimer) + 1) + m_MinTimer;
	int temp = rand() % 2;
	m_SpeedSign = temp == 0 ? -1 : 1;
	m_IsLookingAtPlayer = false;
}

Behaviour* EnemyMove::HandleInput()
{
	bool isDead{ false };
	m_pFSM->GetBlackboard()->GetData("IsDead", isDead);

	if (isDead)
		return m_pLaunchTransition;
	else if (m_IsLookingAtPlayer)
		return m_pAttackTransition;
	else if (m_HasMovedEnough)
		return m_pJumpTransition;

	return nullptr;
}

void EnemyMove::Update(float elapsesSec)
{
	m_IsLookingAtPlayer = false;
	m_Timer += elapsesSec;

	//Move
	m_pRigidbody->MoveHorizontal({ m_Speed * m_SpeedSign, 0 }); // Not * elapsedSec since there is no acceleration

	//Handle Sprite
	if (m_pRigidbody->GetVelocity().x < 0)
		m_pSprite->Flip(true);
	else
		m_pSprite->Flip(false);
	
	//If looking at player
	auto closestObject = Raycast(m_ViewRangePlayer);
	if (closestObject && closestObject->HasTags({ "Player" }))
	{
		m_IsLookingAtPlayer = true;
	}

	//If moved enough
	else if (m_Timer >= m_CurrentMaxTimer)
	{
		m_HasMovedEnough = true;
	}

	//If running against a wall
	closestObject = Raycast(m_ViewRangeWall);
	if (!m_IsLookingAtPlayer && closestObject && closestObject->HasTags({ "Wall" }))
	{
		m_SpeedSign *= -1;
	}

	if (m_pSprite)
		m_pSprite->Update(elapsesSec);
}

void EnemyMove::DrawInterface()
{
	using namespace ImGui;
	Separator();
	Text("Transitions");
	Separator();

	//Set transition, A lot of repetition because of the way imgui works
	if (Button("JumpTransition"))
		m_pJumpTransition = nullptr;
	Behaviour* temp = HandleTransitionDrop(this);
	if (temp)
		m_pJumpTransition = temp;
	PrintTransitionSet(m_pJumpTransition);

	if (Button("ShootTransition"))
		m_pAttackTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pAttackTransition = temp;
	PrintTransitionSet(m_pAttackTransition);

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
	InputFloat("Speed", &m_Speed);
	InputFloat("MaxMoveTimer", &m_MaxTimer);
	InputFloat("MinMoveTimer", &m_MinTimer);
	InputFloat("ViewRangePlayer", &m_ViewRangePlayer);
	InputFloat("ViewRangeWall", &m_ViewRangeWall);
	Separator();
}

void EnemyMove::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	if (m_pJumpTransition)
		node->append_attribute(doc->allocate_attribute("JumpTransition", m_pJumpTransition->GetName().c_str()));
	if (m_pAttackTransition)
		node->append_attribute(doc->allocate_attribute("AttackTransition", m_pAttackTransition->GetName().c_str()));
	if (m_pLaunchTransition)
		node->append_attribute(doc->allocate_attribute("AttackTransition", m_pLaunchTransition->GetName().c_str()));

	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));

	node->append_attribute(doc->allocate_attribute("Speed", FloatToXMLChar(doc, m_Speed)));
	node->append_attribute(doc->allocate_attribute("MaxMoveTimer", FloatToXMLChar(doc, m_MaxTimer)));
	node->append_attribute(doc->allocate_attribute("MinMoveTimer", FloatToXMLChar(doc, m_MinTimer)));
	node->append_attribute(doc->allocate_attribute("ViewRangePlayer", FloatToXMLChar(doc, m_ViewRangePlayer)));
	node->append_attribute(doc->allocate_attribute("ViewRangeWall", FloatToXMLChar(doc, m_ViewRangeWall)));
}

void EnemyMove::SetAttributes(rapidxml::xml_node<>* node)
{
	auto attribute = node->first_attribute("JumpTransition");
	if (attribute != 0)
		m_pJumpTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("AttackTransition");
	if (attribute != 0)
		m_pAttackTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("LaunchTransition");
	if (attribute != 0)
		m_pLaunchTransition = m_pFSM->GetBehaviour(attribute->value());

	attribute = node->first_attribute("Sprite");
	if (attribute != 0)
		m_pSprite = m_pFSM->GetSprite(attribute->value());

	m_Speed = std::stof(node->first_attribute("Speed")->value());
	m_MaxTimer = std::stof(node->first_attribute("MaxMoveTimer")->value());
	m_MinTimer = std::stof(node->first_attribute("MinMoveTimer")->value());
	m_ViewRangePlayer = std::stof(node->first_attribute("ViewRangePlayer")->value());
	m_ViewRangeWall = std::stof(node->first_attribute("ViewRangeWall")->value());
}

void EnemyMove::SetTransitionsAndSprites(const std::vector<Behaviour*>& pTransitions, const std::vector<Sprite*>& pSprites)
{
	if (pTransitions.size() == 3)
	{
		m_pJumpTransition = pTransitions[0];
		m_pAttackTransition = pTransitions[1];
		m_pLaunchTransition = pTransitions[2];
	}

	if (pSprites.size() > 0)
		m_pSprite = pSprites[0];
}

GameObject* EnemyMove::Raycast(float range)
{
	const Vector2f pos = m_pRigidbody->GetPosition();

	auto closestFicture = m_pGameObject->GetScene()->RayCast({ pos.x / M_PPM, pos.y / M_PPM }, { pos.x / M_PPM + range * m_SpeedSign / M_PPM, pos.y / M_PPM });
	if (closestFicture)
		return static_cast<BoxColliderComponent*>(closestFicture->GetUserData())->GetGameObject();

	return nullptr;
}
