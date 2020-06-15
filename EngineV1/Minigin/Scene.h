#pragma once
#include "SceneManager.h"
#include "ContactListener.h"

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

	void InitialAdd(GameObject* pObject);
	virtual void Initialize();
	virtual void Update(float elapsedSec);
	virtual void Render() const;
	void ResetObjects();

	void AddChild(GameObject* pGameObject, GameObject* pBehindObject = nullptr);
	void DetachChild(GameObject* pObject);
	void DeleteChild(GameObject* pObject);

	void AdaptToFullScreen(const Vector2f& ratio);

	void DrawInterfaceObjects();

	const std::string& GetName() const { return m_Name; }
	const std::vector<GameObject*>& GetGameObjects() const { return m_pObjects; }

	b2Fixture* RayCast( const Vector2f& start, const Vector2f& end);
	b2World* GetPhysicsWorld() const { return m_pPhysicsWorld; };

	GameObject* GetOverhead() const { return m_pSceneOverhead; }
	void SetOVerhead(GameObject* pOverhead) { m_pSceneOverhead = pOverhead; }

private: 

	std::vector<GameObject*> m_pObjects = { };
	b2World* m_pPhysicsWorld = nullptr;
	GameObject* m_pToBeAddedChild = nullptr; //Prevent crash from happening: Item would be added to vector while looping over this vector
	GameObject* m_pToBeDeletedChild = nullptr; 
	GameObject* m_pSceneOverhead = nullptr;
	ContactListener* m_pContactListener = nullptr;
	std::string m_Name = {};
	static const Vector4f m_EditorDimensions; //x,y = left bottom | z,w = right top
};

