#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::Scene()
{
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

void Scene::Initialize()
{

}

void Scene::Update(float elapsedSec)
{
	for(auto object : m_pObjects)
	{
		object->Update(elapsedSec);
	}

	if (m_pToBeAddedObject)
	{
		m_pObjects.insert(m_pObjects.begin() + m_pToBeAddedObject->GetIndexInHierarchy(), m_pToBeAddedObject);
		m_pToBeAddedObject = nullptr;
	}
}

void Scene::Render() const
{
	for (unsigned int i{ m_pObjects.size() }; i > 0; i--)
	{
		m_pObjects[i - 1]->Render();
	}
}

void Scene::AddObject(GameObject* pGameObject, GameObject* behindObject)
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
		m_pToBeAddedObject = pGameObject;
	}

	pGameObject->SetParent(nullptr);
}


void Scene::DetachObject(GameObject* pGameObject)
{
	m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), pGameObject), m_pObjects.end());
}

void Scene::DrawInterface()
{
	//if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	//{
	//	ImGui::SetDragDropPayload("GameObject", this, sizeof(*this));
	//	ImGui::Text(&GetName().front());  // Display when moving

	//	if (m_pParent)
	//	{
	//		m_pParent->DetachChild(this);
	//	}
	//	else
	//	{
	//		SceneManager::GetInstance()->GetCurrentScene()->DetachObject(this);
	//	}

	//	ImGui::EndDragDropSource();
	//}

	//ImGui::GetDragDropPayload();


	for (auto object : m_pObjects)
	{
		object->DrawInterfaceScene();
	}
}

