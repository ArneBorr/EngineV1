#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class AttackBehaviour final : public Behaviour
{
public:
	AttackBehaviour();
	~AttackBehaviour();

	void Enter() override;
	void Initialize() override;
	Behaviour* HandleInput() override;
	void Update(float elapsesSec) override;
	void Exit() override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;

private:
	GameObject* m_pProjectile{ nullptr };
	RigidbodyComponent* m_pRigidbody{ nullptr };
	bool m_IsShotFinished{ false };

	void HandleProjectileDrop();
};

