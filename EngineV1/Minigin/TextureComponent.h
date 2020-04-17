#pragma once
#include "BaseComponent.h"

class Texture2D;

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)

class TextureComponent : public BaseComponent
{
public:
	TextureComponent(GameObject* pGameObject, const std::string& texture);

	~TextureComponent() = default;
	TextureComponent(const TextureComponent& other) = delete;
	TextureComponent(TextureComponent&& other) = delete;
	TextureComponent& operator=(const TextureComponent& other) = delete;
	TextureComponent& operator=(TextureComponent&& other) = delete;

	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SetTexture(const std::string& filename);
	void SetPosition(glm::vec3 offset);

private:
	Texture2D* m_pTexture{};
	glm::vec3 m_Offset;
};

