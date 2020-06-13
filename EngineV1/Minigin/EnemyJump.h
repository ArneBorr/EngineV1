#pragma once
#include "JumpBehaviour.h"

class RigidbodyComponent;

class EnemyJump final : public Behaviour
{
public:
	EnemyJump();

	void Initialize() override;
	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsedSec) override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;
	void SetTransitionsAndSprite(const std::vector<Behaviour*>& pTransitions, Sprite* pSprite) override;

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pScanTransition{ nullptr };
	Behaviour* m_pMoveTransition{ nullptr };
	float m_Timer{};
	float m_JumpStrength{ 6.f };
	const float m_Buffer{0.2f};
};

