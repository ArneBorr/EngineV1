#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "Components.h"
#include "imgui.h"

GameObject::GameObject(const std::string& name)
	: m_Name{ name }
{
}

GameObject::~GameObject()
{
	for (unsigned int i{}; i < m_pComponents.size(); i++)
	{
		delete m_pComponents[i];
		m_pComponents[i] = nullptr;
	}
}

void GameObject::Update(float elapsedSec)
{
	for (auto pComp : m_pComponents)
	{
		pComp->Update(elapsedSec);
	}
}

void GameObject::Render() const
{
	for (auto pComp : m_pComponents)
	{
		pComp->Render();
	}
}

void GameObject::DrawInterface()
{
	if (ImGui::CollapsingHeader(&m_Name.front()))
	{
		//List of components
		const char* items[] = { "TransformComponent", "TextureComponent", "TextComponent" };
		static int currentItem = 0;
		ImGui::Combo(" ", &currentItem, items, IM_ARRAYSIZE(items));
		ImGui::SameLine();

		//Add component that is selected
		if (ImGui::Button("Add"))
		{
			BaseComponent* pComponent = nullptr;
			std::string item = items[currentItem];

			if (item == "TransformComponent")
			{
				pComponent = new TransformComponent(this);
			}
			else if (item == "TextureComponent")
			{
				pComponent = new TextureComponent(this, "background.jpg");
			}
			else if (item == "TextComponent")
			{
				pComponent = new TextComponent(this);
			}

			if (pComponent)
				AddComponent(pComponent);
		}
	}
}

void GameObject::AddComponent(BaseComponent* pComponent)
{
	m_pComponents.push_back(pComponent);
}

