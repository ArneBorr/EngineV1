#pragma once
#include "Singleton.h"

class GameObject;
class Script;
class PlayerScript;
class Behaviour;

class GameObjectManager final : public Singleton<GameObjectManager> // Maybe observer pattern? 
{
public:
	~GameObjectManager();

	void Initialize();
	void DrawInterface1()  const;
	void DrawInterface2() ;

	void SetSelectedGameObject(GameObject* pGameObject);
	GameObject* GetSelectedGameObject() const { return m_pSelectedGameObject; }
	Script* GetAndRemoveSelectedScript();
	Behaviour* GetAndRemoveSelectedBehaviour();
	Script* GetScript(const std::string& name);

private:

	void CreateEmptyGameObject() const;
	void CreateCharacter();
	Script* CreateScript(const std::string& name);
	Behaviour* CreateBehaviour(const std::string& name);

	GameObject* m_pSelectedGameObject{ nullptr };
	Script* m_pSelectedScript{ nullptr };
	Behaviour* m_pSelectedBehaviour { nullptr };

	std::vector<Script*> m_pScripts;
	std::vector<Behaviour*> m_pBehaviours;
};

