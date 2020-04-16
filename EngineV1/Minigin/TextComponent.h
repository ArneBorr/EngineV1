#pragma once
#include "BaseComponent.h"
#include <string>

class Font;
class Texture2D;

class TextComponent final : public BaseComponent
{
public:
	TextComponent(std::weak_ptr<GameObject> pGameObject, const std::shared_ptr<Font>& pFont, const std::string& text = "");
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
	std::shared_ptr<Texture2D> m_Texture;
	std::shared_ptr<Font> m_pFont;
	std::string m_Text;
	glm::vec3 m_Offset;
};

