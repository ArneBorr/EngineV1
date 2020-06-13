#include "MiniginPCH.h"
#include "EnemyJump.h"
#include "RigidbodyComponent.h"

EnemyJump::EnemyJump()
{
	m_Name = "EnemyJump";
}

void EnemyJump::Enter()
{
	JumpBehaviour::Enter();
	m_Timer = 0;
}

Behaviour* EnemyJump::HandleInput()
{
	if (m_Timer > m_Buffer&& m_pRigidbody->IsOnGround())
	{
		auto random = rand() % 2;

		if (random == 0)
			return m_pRunTransition;
		else
			return m_pScanTransition;
	}

	return nullptr;
}

void EnemyJump::Update(float elapsedSec)
{
	m_Timer += elapsedSec;
}

void EnemyJump::SetTransitionsAndSprite(const std::vector<Behaviour*>& pTransitions, Sprite* pSprite)
{
	if (pTransitions.size() == 2)
	{
		m_pRunTransition = pTransitions[0];
		m_pScanTransition = pTransitions[1];
	}

	m_pSprite = pSprite;
}

