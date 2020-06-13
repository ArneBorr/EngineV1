#pragma once
#include "Script.h"

class RigidbodyComponent;

class Projectile final : public Script
{
public:
	Projectile();

	void Initialize() override;
	void DrawInterface() override;
	void Update(float elapsedSec) override;
	void OnNotify(const std::string& event, GameObject* pObject, GameObject* collWithObj) override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>*) override;

	void SetGoingLeft(bool left) { m_IsGoingLeft = left; }

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	float m_Speed{};
	float m_Timer{};
	float m_SurvivalTime{ };
	bool m_IsGoingLeft{ false };
};

