#pragma once
#include "BaseComponent.h"

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)



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

