#include "MiniginPCH.h"
#include "EnemyJump.h"
#include "RigidbodyComponent.h"

EnemyJump::EnemyJump()
{
}

Behaviour* EnemyJump::HandleInput()
{
	if (m_Time > m_Buffer&& m_pRigidbody->IsOnGround())
		return m_pRunTransition;

	return nullptr;
}

void EnemyJump::Update(float elapsedSec)
{
	auto vel = m_pRigidbody->GetVelocity();
	m_pRigidbody->SetVelocity({ 0, vel.y });
	m_Time += elapsedSec;
	std::cout << "Jump\n";
}

void EnemyJump::SetTransitionsAndSprite(const std::vector<Behaviour*>& pTransitions, Sprite* pSprite)
{
	if (pTransitions.size() == 1)
	{
		m_pRunTransition = pTransitions[0];
	}

	m_pSprite = pSprite;
}

