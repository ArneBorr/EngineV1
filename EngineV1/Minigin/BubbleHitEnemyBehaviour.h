#pragma once
#include "Behaviour.h"

class BubbleHitEnemyBehaviour : public Behaviour
{
public:
	BubbleHitEnemyBehaviour();

	void Initialize() override;
	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsedSec) override;
	void OnNotify(const std::string& event, GameObject* pObject) override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;

private:
	Behaviour* m_pIdleTransition{ nullptr };
	Behaviour* m_pJumpTransition{ nullptr };
	Behaviour* m_pShootTransition{ nullptr };

	float m_Speed{ 1 };
	float m_MaxSpeed{ 10 };

	int m_SpeedSign = 1;
	bool m_HasMovementInput = false;
};

