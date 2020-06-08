#pragma once
#include "BaseComponent.h"
class MovementComponent final : public BaseComponent
{
public:
	MovementComponent(GameObject* pObject);

	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(float speed, float maxSpeed, float jumpStrength);

private:
	float m_Speed{ 1.f };
	float m_MaxSpeed{ 10.f };
	float m_JumpStrength{ 1000.f };
};

