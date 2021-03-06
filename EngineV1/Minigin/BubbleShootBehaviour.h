#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class BubbleShootBehaviour : public Behaviour
{
public:
	BubbleShootBehaviour();

	void Initialize() override;
	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsedSec) override;
	void Exit() override;
	void OnNotify(const std::string& event, GameObject* pObject, GameObject* collWithObj) override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pBubbleFloat{ nullptr };
	Behaviour* m_pBubbleHit{ nullptr };

	float m_Speed{ 3.3f };
	float m_Timer{ };
	float m_ShootTime{ 1.5f };
	bool m_HasHitEnemy{ false };
};

