#pragma once
#include "BaseComponent.h"
#include <string>


class Font;
class Texture2D;

class TextComponent final : public BaseComponent
{
public:
	TextComponent(GameObject* pGameObject, Font* pFont = nullptr, const std::string& text = "TEMPLATE");
	~TextComponent();
	TextComponent(const TextComponent& other) = delete;
	TextComponent(TextComponent&& other) = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) = delete;

	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(const Vector2f& offset);

	void SetText(const std::string& text);

private:
	void UpdateTexture();

	Texture2D* m_pTexture;
	Font* m_pFont;
	std::string m_Text;
	Vector2f m_Offset;
};

