#pragma once
#include "BaseComponent.h"

class Texture2D;

class TextureComponent : public BaseComponent
{
public:
	TextureComponent(GameObject* pGameObject, const std::string& texturePath);

	~TextureComponent();
	TextureComponent(const TextureComponent& other) = delete;
	TextureComponent(TextureComponent&& other) = delete;
	TextureComponent& operator=(const TextureComponent& other) = delete;
	TextureComponent& operator=(TextureComponent&& other) = delete;

	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;
	void AdaptToFullscreen(const Vector2f& ratio) override;

	void SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode) override;
	void SetAttributes(const Vector2f& offset, int center);

	void SetTexture(const std::string& texture);
	void SetSourceRect(const Vector4f& srcRect) { m_SrcRect = srcRect; }
	void SetFlip(bool flip) { m_Flip = flip; };

private:
	Texture2D* m_pTexture = nullptr;
	std::string m_Path = {};
	Vector4f m_SrcRect = {};
	Vector2f m_Offset = {};
	char m_TexturePathImGui[40] = {};
	bool m_Center = true;
	bool m_Flip = false;
};

