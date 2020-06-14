#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class HitBehaviour : public Behaviour
{
public:
	HitBehaviour();

	void Initialize() override;
	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsedSec) override;
	void Exit() override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pIdleTransition{ nullptr };

	float m_Timer{};
	float m_MaxDeathTime{ 3.f };
};

