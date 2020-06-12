#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class ShootBehaviour : public Behaviour
{
public:
	ShootBehaviour();

	void Enter() override;
	void Initialize() override;
	Behaviour* HandleInput() override;
	void Update(float elapsesSec) override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	bool m_IsShotFinished{ false };
};

