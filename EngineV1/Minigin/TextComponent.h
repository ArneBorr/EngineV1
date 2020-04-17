#pragma once
#include "BaseComponent.h"
#include <string>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)

class Font;
class Texture2D;

class TextComponent final : public BaseComponent
{
public:
	TextComponent(GameObject* pGameObject, Font* pFont = nullptr, const std::string& text = "TEMPLATE");
	~TextComponent() = default;
	TextComponent(const TextComponent& other) = delete;
	TextComponent(TextComponent&& other) = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) = delete;

	void Render() override;
	void Update(float elapsedSec) override;

	void SetText(const std::string& text);
	void SetPosition(glm::vec3 pos);

private:
	void UpdateTexture();
	Texture2D* m_pTexture;
	Font* m_pFont;
	std::string m_Text;
	glm::vec3 m_Offset;
};

