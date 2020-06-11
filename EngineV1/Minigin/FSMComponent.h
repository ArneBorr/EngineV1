#pragma once
#include "BaseComponent.h"

class Behaviour;
class Sprite;

class FSMComponent : public BaseComponent
{
public:
	FSMComponent(GameObject* pObject);
	~FSMComponent();

#pragma region operators
	FSMComponent(const FSMComponent& other) = delete;
	FSMComponent(FSMComponent&& other) = delete;
	FSMComponent& operator=(const FSMComponent& other) = delete;
	FSMComponent& operator=(FSMComponent&& other) = delete;
#pragma endregion

	virtual void Render() override;
	virtual void Update(float elapsedSec) override;
	virtual void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node);

	void SetBehaviours(const std::vector<Behaviour*>& pBehaviours);
	void SetSprites(const std::vector<Sprite*> pSprites);

	Behaviour* GetDraggedBehaviour() const { return m_pDraggedBehaviour; }
	Sprite* GetDraggedSprite() const { return m_pDraggedSprite; }

	Behaviour* GetBehaviour(const std::string& behaviour) const;
	Sprite* GetSprite(const std::string& sprite) const;

private:
	std::vector<Behaviour*> m_pBehaviours;
	std::vector<Sprite*> m_pSprites;
	Behaviour* m_pDraggedBehaviour{ nullptr };
	Sprite* m_pDraggedSprite{ nullptr };

	bool m_IsWindowOpen{ false };

	void DrawFSMTab();
	void DrawSpriteTab();
	void HandleDragSprite(Sprite* pSprite);
	void HandleDropPossibleBehaviour();
	void HandleDragPossibleBehaviour(Behaviour* pBehaviour);
};
