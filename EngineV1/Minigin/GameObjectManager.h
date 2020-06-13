#pragma once
#include "Singleton.h"

class GameObject;
class Script;
class PlayerScript;
class Behaviour;
class SaveHandler;
class Scene;

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
	Behaviour* CreateBehaviour(const std::string& name);
	Script* CreateScript(const std::string& name);
	GameObject* GetPrefab(Scene* pScene, const std::string&) const;
	GameObject* SpawnPrefab(Scene* pScene, const std::string& name, const Vector2f pos) const;

private:

	GameObject* CreateEmptyGameObject() const;

	SaveHandler* m_pSaveHandlerPrefabs{ nullptr };
	GameObject* m_pSelectedGameObject{ nullptr };
	Script* m_pSelectedScript{ nullptr };
	Behaviour* m_pSelectedBehaviour { nullptr };

	std::vector<std::string> m_Prefabs{};
	std::vector<Script*> m_pScripts{};
	std::vector<Behaviour*> m_pBehaviours{};
};

