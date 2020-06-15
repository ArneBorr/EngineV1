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

	virtual void OnNotify(const std::string&, GameObject*, GameObject* ) override {};

	virtual void DrawInterface() = 0;
	virtual void SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode) = 0;
	virtual void SetAttributes(rapidxml::xml_node<>* pNode) = 0;

	const std::string& GetName() const { return m_Name;  }
	void SetGameObject(GameObject* pObject) { m_pGameObject = pObject; }
	void SetFSM(FSMComponent* pFSM) { m_pFSM = pFSM; }

	// Behaviours can override other behaviours in terms of rendering (example, when the player shoots, he can still move/jump etc. but the shooting animation is played)
	bool IsOnTop() const { return m_IsOnTop; } 

protected:
	FSMComponent* m_pFSM = nullptr;
	Sprite* m_pSprite = nullptr;
	GameObject* m_pGameObject = nullptr;
	std::string m_Name;
	bool m_IsOnTop = false;

	Behaviour* HandleTransitionDrop(Behaviour* pThis);
	Sprite* HandleSpriteDrop();
	void PrintTransitionSet(Behaviour* pBehaviour);
	void TransitionButtonInteractions(Behaviour* pBehaviour);
	void GetTransitionsAndSpriteFromAtrribute(std::vector<std::string>& pTransitions, rapidxml::xml_node<>* pNode, std::string& sprite);
};

