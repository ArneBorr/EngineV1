#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"


unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::Scene()
{
}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	m_Objects.push_back(object);
	++m_IdCounter;
}

void Scene::Initialize()
{

}

void Scene::Update(float elapsedSec)
{
	for(auto& object : m_Objects)
	{
		object->Update(elapsedSec);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

