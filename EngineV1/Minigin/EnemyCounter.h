#pragma once
#include "Script.h"
class EnemyCounter final : public Script
{
public:
	EnemyCounter();

	void DrawInterface() override;
	void Update(float elapsedSec) override;
	void OnNotify(const std::string&, GameObject*, GameObject*) override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>*) override;

private:
	float m_AmountOfEnemies{ 4 };
	float m_TimeBeforeRestart{ 3 };
	float m_Timer{};
};

