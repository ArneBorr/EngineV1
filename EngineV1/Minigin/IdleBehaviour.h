#pragma once
#include "Behaviour.h"
class IdleBehaviour final : public Behaviour
{
public:
	IdleBehaviour();

	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsesSec) override;
	void Exit() override;

	void DrawInterface() override;

private:
	Behaviour* m_pRunTransition{ nullptr };
	Behaviour* m_pJumpTransition{ nullptr };
	Behaviour* m_pShootTransition{ nullptr };

};

