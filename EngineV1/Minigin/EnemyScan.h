#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class EnemyScan final : public Behaviour
{
public:
	EnemyScan();

	void Initialize() override;
	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsedSec) override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;
	void SetTransitionsAndSprite(const std::vector<Behaviour*>& pTransitions, Sprite* pSprite) override;

protected:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pRunTransition{ nullptr };

	float m_Timer{ 0 };
	const float m_MaxCheckTime{ 0.2f };
	const float m_ViewRangePlayer{ 150.f };
	bool m_IsLeftChecked{ false };
	bool m_IsRightChecked{ false };
};

