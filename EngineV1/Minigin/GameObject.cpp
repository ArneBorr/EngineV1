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
		//List of components on gameobject
		//static int currentAttachedComptIndex = 0;
		
		//const char* attachedComponents[MAX_COMPONENTS];
		for (unsigned int i{}; i < m_pComponents.size(); i++)
		{
			//attachedComponents[i] = &m_pComponents[i]->GetName().front();
			m_pComponents[i]->DrawInterface();
		}

		//If component is selected, show more options for this component
		/*if (ImGui::ListBox("Attached Components", &currentAttachedComptIndex, attachedComponents, m_pComponents.size()))
		{
			
		}*/

		//List of components that you can add
		const char* PossibleComponents[] = { "TransformComponent", "TextureComponent", "TextComponent" };

		static int currentAddableCompIndex = 0;
		ImGui::Separator();
		ImGui::Combo(" ", &currentAddableCompIndex, PossibleComponents, IM_ARRAYSIZE(PossibleComponents));
		ImGui::SameLine();

		//if clicked, add component that is selected
		if (ImGui::Button("Add"))
		{
			BaseComponent* pComponent = nullptr;
			std::string item = PossibleComponents[currentAddableCompIndex];

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
	//Prevent every component having the same name
	std::string name = pComponent->GetName();
	name += std::to_string(m_pComponents.size());
	pComponent->SetName(name);

	m_pComponents.push_back(pComponent);
}

