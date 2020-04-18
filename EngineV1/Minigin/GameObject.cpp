#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "Components.h"
#include "imgui.h"
#include "Scene.h"
#include "imgui_internal.h"

GameObject::GameObject(const std::string& name)
	: m_Name{ name }
{
}

GameObject::~GameObject()
{
	for (unsigned int i{}; i < m_pChilds.size(); i++)
	{
		delete m_pChilds[i];
		m_pChilds[i] = nullptr;
	}

	for (unsigned int i{}; i < m_pComponents.size(); i++)
	{
		delete m_pComponents[i];
		m_pComponents[i] = nullptr;
	}
}



GameObject::GameObject(GameObject&& other) noexcept
{
	if (this != &other)
	{
		m_pChilds = other.m_pChilds;
		//other.m_pChilds.clear();

		m_Name = other.m_Name;
		other.m_Name.clear();

		m_pParent = other.m_pParent;
		other.m_pParent = nullptr;

		m_pComponents = other.m_pComponents;
		other.m_pComponents.clear();
	}
}

GameObject& GameObject::operator=(GameObject&& other) noexcept
{
	if (this != &other)
	{
		m_pChilds = other.m_pChilds;
		//other.m_pChilds.clear();

		m_Name = other.m_Name;
		other.m_Name.clear();

		m_pParent = other.m_pParent;
		other.m_pParent = nullptr;

		m_pComponents = other.m_pComponents;
		other.m_pComponents.clear();
	}

	return *this;
}

void GameObject::Update(float elapsedSec)
{
	for (auto pComp : m_pComponents)
	{
		pComp->Update(elapsedSec);
	}

	for (auto child : m_pChilds)
	{
		child->Update(elapsedSec);
	}

	if (m_pToBeAddedObject)
	{
		m_pChilds.insert(m_pChilds.begin() + m_pToBeAddedObject->m_IndexInHierarchy, m_pToBeAddedObject);
		m_pToBeAddedObject = nullptr;
	}
}

void GameObject::Render() const
{
	for (auto pComp : m_pComponents)
	{
		pComp->Render();
	}
}

void GameObject::DrawInterfaceScene()
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	bool open = ImGui::TreeNode(&GetName().front());

	// Drag this object to change the parent
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("GameObject", this, sizeof(*this), ImGuiCond_Once);
		ImGui::Text(&m_Name.front());  // Display when moving

		if (m_pParent)
		{
			m_pParent->DetachChild(this);
		}
		else
		{
			SceneManager::GetInstance()->GetCurrentScene()->DetachObject(this);
		}

		ImGui::EndDragDropSource();
	}

	//Drop another object on this to make the object a child
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			auto pObject = new GameObject (std::move(*(GameObject*)(payload->Data)));	
			
			AddChild(pObject);
		}

		ImGui::EndDragDropTarget();
	}
	
	//Show components of this game object when selected
	if (ImGui::IsItemClicked())
		GameObjectManager::GetInstance()->SetSelectedGameObject(this);

	//Show childs
	if (open)
	{
		for (auto gameObject : m_pChilds) // CRASH HERE
		{
			gameObject->DrawInterfaceScene();
		}

		ImGui::TreePop();
	}

	//Space to drop an item in between 2 other items
	ImGui::Selectable("          ", false, 0, { 60, 0.5f });

	//Drop another object on this to change order of parent child
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			auto pObject = new GameObject(std::move(*(GameObject*)(payload->Data)));

			if (m_pParent)
				m_pParent->AddChild(pObject, this);
			else
				SceneManager::GetInstance()->GetCurrentScene()->AddObject(pObject, this);
		}

		ImGui::EndDragDropTarget();
	}
}

void GameObject::DrawInterfaceComponents()
{
	ImGui::Text("Text");
	ImGui::InputText("Text", &m_Name.front(), 128);

	//List of components on gameobject
	for (unsigned int i{}; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->DrawInterface();
	}

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

void GameObject::AddComponent(BaseComponent* pComponent)
{
	//Prevent every component having the same name
	std::string name = pComponent->GetName();
	name += std::to_string(m_pComponents.size());
	pComponent->SetName(name);

	m_pComponents.emplace_back(pComponent);
}

void GameObject::AddChild(GameObject* pGameObject, GameObject* behindObject)
{
	if (behindObject == nullptr)
	{
		pGameObject->SetIndexInHierarchy(m_pChilds.size());
		m_pChilds.emplace_back(pGameObject);
	}
	else
	{
		auto it = std::find(m_pChilds.begin(), m_pChilds.end(), behindObject);
		pGameObject->SetIndexInHierarchy(std::distance(m_pChilds.begin(), it) + 1);
		m_pToBeAddedObject = pGameObject;
	}

	pGameObject->SetParent(this);
}

void GameObject::DetachChild(GameObject* pGameObject)
{
	//remove: It doesn’t actually delete elements from the container but only shunts non-deleted elements forwards on top of deleted elements.
	m_pChilds.erase(std::remove(m_pChilds.begin(), m_pChilds.end(), pGameObject), m_pChilds.end());
	pGameObject->SetParent(nullptr);
}

void GameObject::SetParent(GameObject* pGameObject)
{
	m_pParent = pGameObject;
}

