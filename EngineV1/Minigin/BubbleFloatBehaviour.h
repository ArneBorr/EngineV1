#pragma once
#include "Behaviour.h"


class RigidbodyComponent;

class BubbleFloatBehaviour : public Behaviour
{
public:
	BubbleFloatBehaviour();

	void Initialize() override;
	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsedSec) override;
	void Exit() override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;
	void SetTransitionsAndSprite(const std::vector<Behaviour*>& pTransitions, Sprite* pSprite) override;

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pBubblePop{ nullptr };

	float m_Speed{ 0.5f };
	float m_MaxSpeed{ 2.f };
	float m_Timer{ };
	float m_MoveTime{ 2.5f };
	float m_FloatTime{ 5.f };
};

