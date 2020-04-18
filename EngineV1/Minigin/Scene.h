#pragma once
#include "SceneManager.h"


class GameObject;
class Scene
{
public:

	Scene();

	virtual ~Scene();

	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

	void InitialAdd(GameObject* object); // Used when the gameobject gets added to the scene
	virtual void Initialize() = 0;
	virtual void Update(float elapsedSec);
	virtual void Render() const;

	void AddObject(GameObject* pGameObject, GameObject* behindObject = nullptr); // Used to change hierarchy of Scene
	void DetachObject(GameObject* pObject);

	void DrawInterface();

private: 
	explicit Scene(const std::string& name);

	std::vector<GameObject*> m_pObjects{};
	GameObject* m_pToBeAddedObject{ nullptr }; //Prevent crash from happening: Item would be added to vector while looping over this vector
	std::string m_Name;
};

