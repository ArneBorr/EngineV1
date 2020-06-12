#include "MiniginPCH.h"
#include "ZenChanMove.h"
#include "Sprite.h"
#include "RigidbodyComponent.h"

ZenChanMove::ZenChanMove()
{
	m_Name = "ZenChanMove";
	m_MaxSpeed = 2.f;
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
	if (!m_pRigidbody->IsOnGround())
		return;

	RunBehaviour::Update(elapsesSec);

	m_Timer += elapsesSec;
	const Vector2f pos = m_pRigidbody->GetPosition();

	//If moved enough
	if (m_Timer >= m_CurrentMaxTimer)
		m_HasMovedEnough = true;
	//If running against a wall
	else if (pos.x - m_PreviousPos.x < 0.05f)
	{
		m_SpeedSign *= -1;
		std::cout << "Swap\n";
	}

	if (m_PrevPosTimer > m_PrevPosInterval)
	{
		m_PrevPosTimer = 0;
		m_PreviousPos = pos;
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
