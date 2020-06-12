#pragma once
#include "JumpBehaviour.h"
class EnemyJump final : public JumpBehaviour
{
public:
	EnemyJump();

	Behaviour* HandleInput() override;
	void Update(float) override;

private:
	bool m_IsOnGround{};

};

