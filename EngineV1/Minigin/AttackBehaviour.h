#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class AttackBehaviour : public Behaviour
{
public:
	AttackBehaviour();

	void Enter() override;
	void Initialize() override;
	Behaviour* HandleInput() override;
	void Update(float elapsesSec) override;
	void Exit() override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;
	void SetTransitionsAndSprites(const std::vector<Behaviour*>& pTransitions, const std::vector<Sprite*>& pSprites) override;

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	bool m_IsShotFinished{ false };
};

