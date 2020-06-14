#pragma once
#include "Behaviour.h"
#include "rapidxml.hpp"

class RigidbodyComponent;

class LaunchEnemy : public Behaviour
{
public:
	LaunchEnemy();

	void Initialize() override;
	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsedSec) override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pFallTransition{ nullptr };

	float m_Speed{ 8.f };
	float m_Timer{};
	const float m_MaxRotateTime{ 2.f };
};

