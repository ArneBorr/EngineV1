#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class RunBehaviour final : public Behaviour
{
public:
	RunBehaviour();
	~RunBehaviour() = default;
	
	void Initialize() override;
	Behaviour* HandleInput() override;
	void Update(float elapsesSec) override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;

protected:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pIdleTransition{ nullptr };
	Behaviour* m_pJumpTransition{ nullptr };
	Behaviour* m_pShootTransition{ nullptr };

	float m_Speed{ 1 };

	int m_SpeedSign = 1;
	bool m_HasMovementInput = false;
};

