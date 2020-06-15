#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class IdleBehaviour : public Behaviour
{
public:
	IdleBehaviour();

	void Initialize() override;
	void Enter() override;
	Behaviour* HandleInput() override;
	void OnNotify(const std::string& event , GameObject* pObject , GameObject* pObjCollWith) override;
	void Exit() override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode) override;
	void SetAttributes(rapidxml::xml_node<>* pNode) override;

private:
	RigidbodyComponent* m_pRigidbody = nullptr;
	Behaviour* m_pRunTransition = nullptr;
	Behaviour* m_pJumpTransition = nullptr;
	Behaviour* m_pShootTransition = nullptr;
	Behaviour* m_pHitTransition = nullptr;

	bool m_IsHit = false;
};

