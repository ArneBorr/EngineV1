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
	void OnNotify(const std::string&, GameObject*, GameObject*) override;
	void Exit() override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;

protected:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pRunTransition{ nullptr };
	Behaviour* m_pAttackTransition{ nullptr };
	Behaviour* m_pLaunchTransition{ nullptr };

	float m_Timer{ 0 };
	float m_MaxCheckTime{ 0.2f };
	float m_ViewRangePlayer{ 150.f };
	bool m_IsLeftChecked{ false };
	bool m_IsRightChecked{ false };

	bool IsLookingAtPlayer(float range);
};

