#pragma once
#include "BaseComponent.h"

class Texture2D;

class TextureComponent : public BaseComponent
{
public:
	TextureComponent(GameObject* pGameObject, const std::string& texture);

	~TextureComponent();
	TextureComponent(const TextureComponent& other) = delete;
	TextureComponent(TextureComponent&& other) = delete;
	TextureComponent& operator=(const TextureComponent& other) = delete;
	TextureComponent& operator=(TextureComponent&& other) = delete;

	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(const Vector2f& offset, int center);

	void SetTexture(const std::string& texture);
	void SetSourceRect(const Vector4f& srcRect) { m_SrcRect = srcRect; }

private:
	Texture2D* m_pTexture{};
	std::string m_Path{};
	Vector4f m_SrcRect{};
	Vector2f m_Offset{};
	char m_TexturePathImGui[40]{};
	bool m_Center = true;
};

