#include "MiniginPCH.h"
#include "ZenChanMove.h"
#include "Sprite.h"
#include "RigidbodyComponent.h"
#include "Scene.h"
#include "BoxColliderComponent.h"

ZenChanMove::ZenChanMove()
{
	m_Name = "ZenChanMove";
	m_Speed = 2.4f;
}

void ZenChanMove::Enter()
{
	m_Timer = 0;
	m_pSprite->Reset();
	m_pSprite->Play(true);
	m_HasMovedEnough = false;
	m_CurrentMaxTimer = std::rand() % int((m_TotalMaxTimer - m_MinTimer) + 1) + m_MinTimer;
	int temp = rand() % 2;
	m_SpeedSign = temp == 0 ? -1 : 1;
	m_HasMovementInput = true;
}

Behaviour* ZenChanMove::HandleInput()
{
	if (m_HasMovedEnough)
		return m_pJumpTransition;

	return nullptr;
}

void ZenChanMove::Update(float elapsesSec)
{
	RunBehaviour::Update(elapsesSec);

	m_IsLookingAtPlayer = false;
	m_Timer += elapsesSec;
	const Vector2f pos = m_pRigidbody->GetPosition();

	auto closestFicture = m_pGameObject->GetScene()->RayCast({ pos.x / M_PPM, pos.y / M_PPM }, { pos.x / M_PPM + m_ViewRangePlayer * m_SpeedSign / M_PPM, pos.y / M_PPM });
	GameObject* closesttObject{ nullptr };
	if (closestFicture)
		closesttObject = static_cast<BoxColliderComponent*>(closestFicture->GetUserData())->GetGameObject();
	
	//If looking at player
	if (closesttObject && closesttObject->HasTags({ "Player" }))
	{
		m_IsLookingAtPlayer = true;
		std::cout << "Player\n";
		return;
	}

	//If moved enough
	if (m_Timer >= m_CurrentMaxTimer)
	{
		m_HasMovedEnough = true;
		return;
	}

	//If running against a wall
	closesttObject = nullptr;
	closestFicture = m_pGameObject->GetScene()->RayCast({ pos.x / M_PPM, pos.y / M_PPM }, { pos.x / M_PPM + m_ViewRangeWall * m_SpeedSign / M_PPM, pos.y / M_PPM });
	if (closestFicture)
		closesttObject = static_cast<BoxColliderComponent*>(closestFicture->GetUserData())->GetGameObject();
	if (closesttObject && closesttObject->HasTags({ "Wall" }))
	{
		m_SpeedSign *= -1;
		return;
	}
}

void ZenChanMove::SetTransitionsAndSprite(const std::vector<Behaviour*>& pTransitions, Sprite* pSprite)
{
	if (pTransitions.size() == 1)
	{
		m_pJumpTransition = pTransitions[0];
	}

	m_pSprite = pSprite;
}
