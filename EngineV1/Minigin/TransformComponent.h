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
	void SetAttributes(const Vector2f& pos, const Vector2f& scale, float rot);

	void SetPosition(float x, float y);
	void SetPosition(const Vector2f& pos);
	void SetRotation(float rot);
	void SetScale(float x, float y);
	void SetScale(const Vector2f& scale);

	const Vector2f& GetPosition() const { return m_Position; }
	const Vector2f& GetWorldPosition() const { return m_WorldPosition; }
	const Vector2f& GetScale() const { return m_Scale; }
	const Vector2f& GetWorldScale() const { return m_WorldScale; }
	float GetRotation() const { return m_Rotation; }
	float GetWorldRotation() const { return m_WorldRotation; }

	void UpdateTransform(bool updateBody);

private:
	Vector2f m_Position{};
	Vector2f m_WorldPosition{};
	Vector2f m_Scale{1, 1};
	Vector2f m_WorldScale{1, 1};
	float m_Rotation{};
	float m_WorldRotation{};
};

