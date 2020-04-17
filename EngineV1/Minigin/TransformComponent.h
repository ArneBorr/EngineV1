#pragma once
#include "BaseComponent.h"


class TransformComponent : public BaseComponent
{
public:
	TransformComponent(GameObject* pGameObject);

	void Render() override {};
	void Update(float elapsedSec) override { UNREFERENCED_PARAMETER(elapsedSec); };
	void DrawInterface() override;

	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(float x, float y, float z);
private:
	glm::vec3 m_Position{};
};

