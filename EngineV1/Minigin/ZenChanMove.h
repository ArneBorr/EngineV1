#pragma once
#include "RunBehaviour.h"

class ZenChanMove final : public RunBehaviour
{
public:
	ZenChanMove();

	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsesSec) override;

private:
	Vector2f m_PreviousPos{};
	float m_Timer{ 0 };
	float m_TotalMaxTimer{ 2.5f };
	float m_CurrentMaxTimer{ 1.5f };
	float m_MinTimer{ 1.f };
	bool m_HasMovedEnough{ false };
};

