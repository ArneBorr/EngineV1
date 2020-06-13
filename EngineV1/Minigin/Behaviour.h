#pragma once

#include "Observer.h"

class Sprite;
class FSMComponent;
class GameObject;

class Behaviour : public Observer
{
public:
	Behaviour(const std::string& name);
	virtual ~Behaviour();

	virtual void Initialize() {};
	virtual void Enter();
	virtual Behaviour* HandleInput() = 0;
	virtual void Update(float elapsesSec);
	virtual void Render();
	virtual void Exit() {};

	virtual void OnNotify(const std::string&, GameObject*) override {};

	virtual void DrawInterface() = 0;
	virtual void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) = 0;
	virtual void SetAttributes(rapidxml::xml_node<>* node) = 0;

	const std::string& GetName() const { return m_Name;  }
	void SetGameObject(GameObject* pObject) { m_pGameObject = pObject; }
	void SetFSM(FSMComponent* pFSM) { m_pFSM = pFSM; }
	bool IsOnTop() const { return m_IsOnTop; }
	virtual void SetTransitionsAndSprite(const std::vector<Behaviour*>& pTransitions, Sprite* pSprite);

protected:
	FSMComponent* m_pFSM{ nullptr };
	Sprite* m_pSprite{ nullptr };
	GameObject* m_pGameObject{ nullptr };
	std::string m_Name;
	bool m_IsOnTop{ false };

	Behaviour* HandleTransitionDrop(Behaviour* pThis);
	Sprite* HandleSpriteDrop();
	void PrintTransitionSet(Behaviour* pBehaviour);
	void TransitionButtonInteractions(Behaviour* pBehaviour);
	void GetTransitionsAndSpriteFromAtrribute(std::vector<std::string>& transitions, rapidxml::xml_node<>* node, std::string& sprite);
};

