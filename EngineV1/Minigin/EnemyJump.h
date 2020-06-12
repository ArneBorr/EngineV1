#pragma once
#include "JumpBehaviour.h"
class EnemyJump final : public JumpBehaviour
{
public:
	EnemyJump();

	Behaviour* HandleInput() override;
	void Update(float elapsedSec) override;
	void SetTransitionsAndSprite(const std::vector<Behaviour*>& pTransitions, Sprite* pSprite) override;

private:
	float m_Time{};
	const float m_Buffer{0.2f};

};

