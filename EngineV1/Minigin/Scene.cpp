#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"
#include <algorithm> 


const Vector4f Scene::m_EditorDimensions = Vector4f{ 255, 100, 1026, 536 }; //848x480


Scene::Scene(const std::string& name)
	: m_Name{ name }
{
	m_pPhysicsWorld = new b2World(b2Vec2(0, -9.81f));
}

Scene::~Scene() = default;

void Scene::InitialAdd(GameObject* pGameObject)
{
	//Prevents every gameobject getting the same name
	pGameObject->SetIndexInHierarchy(m_pObjects.size());
	std::string name = pGameObject->GetName();
	name += std::to_string(m_pObjects.size());
	pGameObject->SetName(name);
	m_pObjects.push_back(pGameObject);
}

void Scene::Update(float elapsedSec)
{
	if (m_pToBeDeletedChild)
	{
		DetachChild(m_pToBeDeletedChild);
		delete m_pToBeDeletedChild;
		m_pToBeDeletedChild = nullptr;
	}

	for(auto object : m_pObjects)
	{
		object->Update(elapsedSec);
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

void Scene::ChangeGameobjectsToFullscreen()
{
	for (auto object : m_pObjects)
		object->ChangeToFullScreen();
}

void Scene::DrawInterface()
{
	ImGui::SetNextWindowPos({ m_EditorDimensions.x , m_EditorDimensions.y }, ImGuiCond_Always);
	ImGui::SetNextWindowSize({ m_EditorDimensions.z - m_EditorDimensions.x,  m_EditorDimensions.w - m_EditorDimensions.y });
	ImGui::GetStyle().Alpha = 0.01f;
	ImGui::Begin("Scene");
	ImGui::End();
	ImGui::GetStyle().Alpha = 1.f;
}

void Scene::DrawInterfaceObjetcs()
{
	for (auto object : m_pObjects)
	{
		object->DrawInterfaceScene();
	}
}

