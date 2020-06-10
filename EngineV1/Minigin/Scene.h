#pragma once
#include "SceneManager.h"

class GameObject;
class b2World;
class Scene
{
public:

	Scene(const std::string& name);

	virtual ~Scene();

	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

	void InitialAdd(GameObject* object); // Used when the gameobject gets added to the scene
	virtual void Initialize();
	virtual void Update(float elapsedSec);
	virtual void Render() const;
	void ResetObjects();

	void AddChild(GameObject* pGameObject, GameObject* behindObject = nullptr); // Used to change hierarchy of Scene
	void DetachChild(GameObject* pObject);
	void DeleteChild(GameObject* pObject);

	void ChangeGameobjectsToFullscreen();

	void DrawInterfaceObjects();

	const std::string& GetName() const { return m_Name; }
	const std::vector<GameObject*>& GetGameObjects() const { return m_pObjects; }
	b2World* GetPhysicsWorld() const { return m_pPhysicsWorld; };

private: 

	std::vector<GameObject*> m_pObjects;
	b2World* m_pPhysicsWorld{ nullptr };
	GameObject* m_pToBeAddedChild{ nullptr }; //Prevent crash from happening: Item would be added to vector while looping over this vector
	GameObject* m_pToBeDeletedChild{ nullptr }; // Beytter way to do this (Deletion child)
	std::string m_Name;
	static const Vector4f m_EditorDimensions; //x,y = left bottom | z,w = right top
};

