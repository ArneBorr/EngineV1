#pragma once
#include "Singleton.h"

class GameObject;
class Script;
class PlayerScript;

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
	Script* GetScript(const std::string& name);

private:

	void CreateEmptyGameObject() const;
	Script* CreateScript(const std::string& name);

	GameObject* m_pSelectedGameObject{ nullptr };
	Script* m_pSelectedScript{ nullptr };

	std::vector<Script*> m_pScripts;
};

