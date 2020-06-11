#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class JumpBehaviour : public Behaviour
{
public:
	JumpBehaviour();

	void Initialize() override;
	void Enter() override;
	Behaviour* HandleInput() override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pIdleTransition{ nullptr };
	Behaviour* m_pRunTransition{ nullptr };
	Behaviour* m_pShootTransition{ nullptr };

	float m_JumpStrength{ 30.f };
};

