#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class JumpBehaviour : public Behaviour
{
public:
	JumpBehaviour();

	virtual void Initialize() override;
	virtual void Enter() override;
	virtual Behaviour* HandleInput() override;

	virtual void DrawInterface() override;
	virtual void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	virtual void SetAttributes(rapidxml::xml_node<>* node) override;

protected:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pIdleTransition{ nullptr };
	Behaviour* m_pRunTransition{ nullptr };
	Behaviour* m_pAttackTransition{ nullptr };

	float m_JumpStrength{ 30.f };
};

