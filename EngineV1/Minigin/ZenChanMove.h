#pragma once
#include "RunBehaviour.h"

class ZenChanMove final : public RunBehaviour
{
public:
	ZenChanMove();

	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsesSec) override;
	void SetTransitionsAndSprite(const std::vector<Behaviour*>& pTransitions, Sprite* pSprite) override;

private:
	Vector2f m_PreviousPos{};
	float m_Timer{ 0 };
	float m_PrevPosTimer{ 0 };
	float m_CurrentMaxTimer{ 1.5f };
	const float m_TotalMaxTimer{ 3.5f };
	const float m_MinTimer{ 2.f };
	const float m_PrevPosInterval{ 0.05f };
	const float m_ViewRangePlayer{ 150.f };
	const float m_ViewRangeWall{ 25.f };
	bool m_HasMovedEnough{ false };
	bool m_IsLookingAtPlayer{ false };
};

