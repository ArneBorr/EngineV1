#pragma once
#include "BaseComponent.h"

class Texture2D;

class TextureComponent : public BaseComponent
{
public:
	TextureComponent(std::weak_ptr<GameObject> pGameObject, const std::string& texture);

	~TextureComponent() = default;
	TextureComponent(const TextureComponent& other) = delete;
	TextureComponent(TextureComponent&& other) = delete;
	TextureComponent& operator=(const TextureComponent& other) = delete;
	TextureComponent& operator=(TextureComponent&& other) = delete;

	void Render() override;
	void Update(float elapsedSec) override;
	void SetTexture(const std::string& filename);
	void SetPosition(glm::vec3 offset);

private:
	std::shared_ptr<Texture2D> m_pTexture{};
	glm::vec3 m_Offset;
};

