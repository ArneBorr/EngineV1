#pragma once
#include "BaseComponent.h"
#include "Sprite.h"


class AnimatorControllerComponent final : public BaseComponent
{
public:
	AnimatorControllerComponent(GameObject* pObject);
	~AnimatorControllerComponent();

	void Render() override;
	void Update(float elapsedSec) override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(const std::vector<Sprite*>& sprites);

private:
	std::vector<Sprite*> m_pSprites;
	unsigned int m_ActiveSprite = 0;
	bool m_IsWindowOpen = false;
};

