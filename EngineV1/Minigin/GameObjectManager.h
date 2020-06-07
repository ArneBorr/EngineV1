#pragma once
#include "Singleton.h"

class GameObject;

class GameObjectManager final : public Singleton<GameObjectManager>// Maybe observer pattern? 
{
public:
	~GameObjectManager() = default;

	void DrawInterface1()  const;
	void DrawInterface2()  const;

	void SetSelectedGameObject(GameObject* pGameObject);

private:
	void CreateEmptyGameObject() const;

	GameObject* m_pSelectedGameObject{ nullptr };
};

