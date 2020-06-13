#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class BubbleHitEnemyBehaviour : public Behaviour
{
public:
	BubbleHitEnemyBehaviour();

	void Initialize() override;
	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsedSec) override;
	virtual void Render();
	void OnNotify(const std::string& event, GameObject* pObject, GameObject* pTrigger) override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pPopTransition{ nullptr };
	Sprite* m_pOtherEnemySprite{ nullptr };

	float m_Speed{ 2.f };
	float m_Timer{ };
	float m_MoveTime{ 2.5f };
	float m_FloatTime{ 7.f };
	bool m_IsZenChan{ true };
};

