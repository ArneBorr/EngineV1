#pragma once
#include "BaseComponent.h"
#include "Observer.h"

class Behaviour;
class Sprite;
class Blackboard;
class GameObject;

class FSMComponent final : public BaseComponent, public Observer
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

	void Initialize() override;
	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node);
	void OnNotify(const std::string& event, GameObject* pObject, GameObject* trigger) override;

	void SetBehaviours(const std::vector<Behaviour*>& pBehaviours);
	void SetSprites(const std::vector<Sprite*> pSprites);
	void LoadSettings(const std::string& name);

	Behaviour* GetDraggedBehaviour() const { return m_pDraggedBehaviour; }
	Sprite* GetDraggedSprite() const { return m_pDraggedSprite; }

	Behaviour* GetBehaviour(const std::string& behaviour) const;
	Sprite* GetSprite(const std::string& sprite) const;

	void SetStartingState(unsigned int i) { m_StartingBehaviourIndex = i; }
	Blackboard* GetBlackboard() const { return m_pBlackboard; }

	void Pause(bool pause) { m_IsFSMPaused = pause; }
	bool IsPaused() const { return m_IsFSMPaused; }

private:
	std::vector<Behaviour*> m_pBehaviours;
	std::vector<Sprite*> m_pSprites;
	Blackboard* m_pBlackboard{ nullptr };
	Behaviour* m_pCurrentBehaviour{ nullptr };
	Behaviour* m_pOnTopBehaviour{ nullptr };
	Behaviour* m_pDraggedBehaviour{ nullptr };
	Sprite* m_pDraggedSprite{ nullptr };
	unsigned int m_StartingBehaviourIndex{ 0 };
	bool m_IsWindowOpen{ false };
	bool m_IsFSMPaused{ false };

	void DrawFSMTab();
	void DrawSpriteTab();
	void HandleDragSprite(Sprite* pSprite);
	void HandleDropPossibleBehaviour();
	void HandleDragPossibleBehaviour(Behaviour* pBehaviour);
	void LoadPlayerSettings();
	void LoadBubbleSettings();
	void LoadZenChanSettings();
};

