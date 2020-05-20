#pragma once
#include "BaseComponent.h"

class Texture2D;

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

	void SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node) override;

	void SetTexture(const std::string& filename);
	void SetPosition(float x, float y);
	void SetPosition(const Vector2f& offset);

private:
	Texture2D* m_pTexture{};
	std::string m_Path;
	Vector2f m_Offset;
};

