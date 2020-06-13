#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"
#include <algorithm> 
#include "RayCastCallback.h"

const Vector4f Scene::m_EditorDimensions = Vector4f{ 255, 100, 1026, 536 }; //848x480


Scene::Scene(const std::string& name)
	: m_Name{ name }
	, m_ContactListener{}
{
	m_pPhysicsWorld = new b2World(b2Vec2(0, 9.81f));
	m_pPhysicsWorld->SetContactListener(&m_ContactListener);
}

Scene::~Scene()
{
	for (auto object : m_pObjects)
	{
		delete object;
		object = nullptr;
	}
	
	delete m_pPhysicsWorld;
	m_pPhysicsWorld = nullptr;
}

void Scene::InitialAdd(GameObject* pGameObject)
{
	//Prevents every gameobject getting the same name
	pGameObject->SetIndexInHierarchy(m_pObjects.size());
	std::string name = pGameObject->GetName();
	name += std::to_string(m_pObjects.size());
	pGameObject->SetName(name);
	pGameObject->SetScene(this);
	m_pObjects.push_back(pGameObject);
}

void Scene::Initialize()
{
	for (auto object : m_pObjects)
		object->Initialize();
}

void Scene::Update(float elapsedSec)
{
	if (m_pToBeDeletedChild)
	{
		DetachChild(m_pToBeDeletedChild);
		delete m_pToBeDeletedChild;
		m_pToBeDeletedChild = nullptr;
	}

	static int velocityIterations = 8;
	static int positionIterations = 3;
	if (GameInfo::GetInstance()->IsPlaying())
		m_pPhysicsWorld->Step(1 / 60.f, velocityIterations, positionIterations);

	const int size = m_pObjects.size();
	for (int i{}; i < size; i++)
	{
		m_pObjects[i]->Update(elapsedSec);
	}

	for (auto object : m_pObjects)
	{
		object->LateUpdate();
	}

	if (m_pToBeAddedChild)
	{
		m_pObjects.insert(m_pObjects.begin() + m_pToBeAddedChild->GetIndexInHierarchy(), m_pToBeAddedChild);
		m_pToBeAddedChild = nullptr;
	}
}

void Scene::Render() const
{
	for (unsigned int i{ m_pObjects.size() }; i > 0; i--)
	{
		m_pObjects[i - 1]->Render();
	}
}

void Scene::ResetObjects()
{
	for (auto pObject : m_pObjects)
		pObject->Reset();
}

void Scene::AddChild(GameObject* pGameObject, GameObject* behindObject)
{
	if (behindObject == nullptr)
	{
		pGameObject->SetIndexInHierarchy(m_pObjects.size());
		m_pObjects.emplace_back(pGameObject);
	}
	else
	{
		auto it = std::find(m_pObjects.begin(), m_pObjects.end(), behindObject);
		pGameObject->SetIndexInHierarchy(std::distance(m_pObjects.begin(), it) + 1);
		m_pToBeAddedChild = pGameObject;
	}

	pGameObject->SetParent(nullptr);
	pGameObject->SetScene(this);
}


void Scene::DetachChild(GameObject* pGameObject)
{
	m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), pGameObject), m_pObjects.end());
}

void Scene::DeleteChild(GameObject* pObject)
{
	m_pToBeDeletedChild = pObject;
	GameObjectManager::GetInstance()->SetSelectedGameObject(nullptr);
}

void Scene::AdaptToFullScreen(const Vector2f& ratio)
{
	for (auto object : m_pObjects)
		object->AdaptToFullScreen(ratio);
}

void Scene::DrawInterfaceObjects()
{
	for (auto object : m_pObjects)
	{
		object->DrawInterfaceScene();
	}
}

b2Fixture* Scene::RayCast(const Vector2f& start, const Vector2f& end)
{
	RayCastCallback callback{ };

	m_pPhysicsWorld->RayCast(&callback, { start.x, start.y }, { end.x, end.y });
	return  callback.GetClosestFixture();
}

