#pragma once
#include "Behaviour.h"

class RigidbodyComponent;

class JumpBehaviour final : public Behaviour
{
public:
	JumpBehaviour();

	void Initialize() override;
	void Enter() override;
	Behaviour* HandleInput() override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;
	void SetTransitionsAndSprites(const std::vector<Behaviour*>& pTransitions, const std::vector<Sprite*>& pSprites) override;

protected:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Behaviour* m_pIdleTransition{ nullptr };
	Behaviour* m_pRunTransition{ nullptr };
	Behaviour* m_pAttackTransition{ nullptr };

	float m_JumpStrength{ 6.f };
};

