#include "MiniginPCH.h"
#include "ZenChanMove.h"
#include "Sprite.h"
#include "RigidbodyComponent.h"

ZenChanMove::ZenChanMove()
{
	m_Name = "ZenChanMove";
}

void ZenChanMove::Enter()
{
	m_Timer = 0;
	m_pSprite->Reset();
	m_pSprite->Play(true);
	m_HasMovedEnough = false;
	m_CurrentMaxTimer = std::rand() % int((m_TotalMaxTimer - m_MinTimer) + 1) + m_MinTimer;
	m_SpeedSign = rand() % 2;
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

	m_Timer += elapsesSec;
	const Vector2f pos = m_pRigidbody->GetPosition();

	//If moved enough
	if (m_Timer >= m_CurrentMaxTimer)
		m_HasMovedEnough = true;
	//If running against a wall
	else if (pos.x - m_PreviousPos.x < FLT_EPSILON)
		m_SpeedSign *= -1;

	m_PreviousPos = pos;
}
