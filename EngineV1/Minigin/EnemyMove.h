#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class EnemyMove : public Behaviour
{
public:
	EnemyMove();

	void Initialize() override;
	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsesSec) override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;
	void SetTransitionsAndSprites(const std::vector<Behaviour*>& pTransitions, const std::vector<Sprite*>& pSprites) override;

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pJumpTransition{ nullptr };
	Behaviour* m_pAttackTransition{ nullptr };

	float m_Speed{ 6 };
	float m_Timer{ 0 };
	float m_CurrentMaxTimer{ 1.5f };
	float m_MaxTimer{ 3.5f };
	float m_MinTimer{ 2.f };
	float m_ViewRangePlayer{ 150.f };
	float m_ViewRangeWall{ 25.f };
	int m_SpeedSign{ 1 };
	bool m_HasMovedEnough{ false };
	bool m_IsLookingAtPlayer{ false };

	GameObject* Raycast(float range);
};

