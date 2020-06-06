#pragma once
#include "BaseComponent.h"


class TransformComponent final : public BaseComponent
{
public:
	TransformComponent(GameObject* pGameObject);

	void Render() override {};
	void Update(float elapsedSec) override { UNREFERENCED_PARAMETER(elapsedSec); };
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node) override;

	void SetPosition(float x, float y);
	void SetPosition(const Vector2f& pos);
	void SetRotation(float rot);
	void SetScale(float x, float y);
	void SetScale(const Vector2f& scale);

	const Vector2f& GetPosition() const { return m_Position; }
	const Vector2f& GetScale() const { return m_Scale; }
	float GetRotation() const { return m_Rotation; }

private:
	Matrix3f m_WorldMatrix{};
	Vector2f m_Position{};
	float m_Rotation{};
	Vector2f m_Scale{1, 1};
};

