#pragma once
#include "JumpBehaviour.h"
class EnemyJump final : public JumpBehaviour
{
public:
	EnemyJump();

	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsedSec) override;
	void SetTransitionsAndSprite(const std::vector<Behaviour*>& pTransitions, Sprite* pSprite) override;

private:
	Behaviour* m_pScanTransition{ nullptr };
	float m_Timer{};
	const float m_Buffer{0.2f};

};

