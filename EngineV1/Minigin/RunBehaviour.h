#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class RunBehaviour final : public Behaviour
{
public:
	RunBehaviour();
	~RunBehaviour() = default;
	
	void Initialize() override;
	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsesSec) override;
	void OnNotify(const std::string&, GameObject*, GameObject*) override;
	void Exit() override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;

protected:
	RigidbodyComponent* m_pRigidbody = nullptr;
	Behaviour* m_pIdleTransition = nullptr;
	Behaviour* m_pJumpTransition = nullptr;
	Behaviour* m_pShootTransition = nullptr;
	Behaviour* m_pHitTransition = nullptr;

	float m_Speed = 1;

	int m_SpeedSign = 1;
	bool m_HasMovementInput = false;
	bool m_IsHit = false;
};

