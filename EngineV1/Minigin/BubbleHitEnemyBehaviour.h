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
	void Exit() override;
	void Render();
	void OnNotify(const std::string& event, GameObject* pObject, GameObject* pTrigger) override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pPopTransition{ nullptr };
	Sprite* m_pMaitaSprite{ nullptr };
	Sprite* m_pSpriteToRender{ nullptr };

	float m_Speed{ 2.f };
	float m_Timer{ };
	float m_MoveTime{ 1.f };
	float m_FloatTime{ 7.f };
	bool m_IsZenChan{ true };
	bool m_IsCaught{ false };
};

