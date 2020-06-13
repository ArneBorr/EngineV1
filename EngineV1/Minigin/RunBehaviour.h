#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class RunBehaviour : public Behaviour
{
public:
	RunBehaviour();
	virtual ~RunBehaviour() = default;

	virtual void Initialize() override;
	virtual Behaviour* HandleInput() override;
	virtual void Update(float elapsesSec) override;
	virtual void Exit() override;

	virtual void DrawInterface() override;
	virtual void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	virtual void SetAttributes(rapidxml::xml_node<>* node) override;

protected:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pIdleTransition{ nullptr };
	Behaviour* m_pJumpTransition{ nullptr };
	Behaviour* m_pShootTransition{ nullptr };

	float m_Speed{ 1 };

	int m_SpeedSign = 1;
	bool m_HasMovementInput = false;
};

