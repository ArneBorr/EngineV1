#pragma once

class Sprite;
class FSMComponent;

class Behaviour
{
public:
	Behaviour(const std::string& name);
	virtual ~Behaviour();

	virtual void Enter() {};
	virtual Behaviour* HandleInput() = 0;
	virtual void Update(float elapsesSec) = 0;
	virtual void Exit() {};

	virtual void DrawInterface() = 0;

	const std::string& GetName() const { return m_Name;  }
	void SetGameObject(GameObject* pObject) { m_pGameObject = pObject; }
	void SetFSM(FSMComponent* pFSM) { m_pFSM = pFSM; }

protected:
	FSMComponent* m_pFSM{ nullptr };
	Sprite* m_pSprite{ nullptr };
	GameObject* m_pGameObject{ nullptr };
	std::string m_Name;

	Behaviour* HandleTransitionDrop(Behaviour* pThis);
	Sprite* HandleSpriteDrop();
	void PrintTransitionSet(Behaviour* pBehaviour);
	void TransitionButtonInteractions(Behaviour* pBehaviour);
};

