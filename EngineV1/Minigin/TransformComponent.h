#pragma once
#include "BaseComponent.h"


class TransformComponent : public BaseComponent
{
public:
	TransformComponent(GameObject* pGameObject);

	void Render() override {};
	void Update(float elapsedSec) override { UNREFERENCED_PARAMETER(elapsedSec); };
	void DrawInterface() override;

	void SetPosition(float x, float y);
	void SetPosition(Vector2f pos);
	void SetScale(float x, float y);
	void SetScale(Vector2f scale);

	const Vector2f& GetPosition() const { return m_Position; }
	const Vector2f& GetScale() const { return m_Scale; }

private:
	Vector2f m_Position{};
	Vector2f m_Scale{1, 1};
};

