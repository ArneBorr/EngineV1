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
	void SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode) override;
	void SetAttributes(const std::vector<Sprite*>& pSprites);

	void Play(bool play) { m_IsPlaying = play; };

private:
	std::vector<Sprite*> m_pSprites;
	unsigned int m_ActiveSprite = 0;
	bool m_IsWindowOpen = false;
	bool m_IsPlaying = false;;

};

