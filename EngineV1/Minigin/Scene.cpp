#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "imgui.h"

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::Scene()
{
}

Scene::~Scene() = default;

void Scene::Add(GameObject* object)
{
	//Prevents every gameobject getting the same name
	std::string name = object->GetName();
	name += std::to_string(m_pObjects.size());
	object->SetName(name);
	m_pObjects.push_back(object);
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
}

void Scene::Render() const
{
	for (auto object : m_pObjects)
	{
		object->Render();
	}
}

void Scene::DrawInterface() const
{
	for (auto object : m_pObjects)
	{
		if (ImGui::CollapsingHeader(&object->GetName().front()))
		{
			const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
			static int item_current = 0;
			ImGui::Combo(" ", &item_current, items, IM_ARRAYSIZE(items));
			ImGui::SameLine();
			if (ImGui::Button("Add"))
			{

			}
		}
	}
}

