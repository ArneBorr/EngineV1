#pragma once
#include "BaseComponent.h"
#include "RunBehaviour.h"
class MovementComponent final : public BaseComponent
{
public:
	MovementComponent(GameObject* pObject);

	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(float speed, float jumpStrength);

	void LoadSettings(const std::string settings);

private:
	friend class RunBehaviour;

	float m_Speed{ 1.f };
	float m_JumpStrength{ 1000.f };

	void LoadPlayerSettings();
};

