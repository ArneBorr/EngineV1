#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "Components.h"
#include "imgui.h"
#include "Scene.h"
#include "imgui_internal.h"
#include <algorithm>

GameObject::GameObject(const std::string& name)
	: m_Name{ name }
{
}

GameObject::~GameObject()
{
	for (unsigned int i{}; i < m_pChildren.size(); i++)
	{
		delete m_pChildren[i];
		m_pChildren[i] = nullptr;
	}

	for (unsigned int i{}; i < m_pComponents.size(); i++)
	{
		delete m_pComponents[i];
		m_pComponents[i] = nullptr;
	}

	m_pScene = nullptr;
	m_pParent = nullptr;
}

GameObject::GameObject(GameObject&& other) noexcept
{
	if (this != &other)
	{
		m_pChildren = other.m_pChildren;
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
		m_pChildren = other.m_pChildren;
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
	if (m_pToBeDetachedChild)
	{
		DetachChild(m_pToBeDetachedChild);
		delete m_pToBeDetachedChild;
		m_pToBeDetachedChild = nullptr;
	}

	for (auto pComp : m_pComponents)
	{
		pComp->Update(elapsedSec);
	}

	for (auto child : m_pChildren)
	{
		child->Update(elapsedSec);
	}

	// Change hierachy when needed
	if (m_pToBeAddedObject)
	{
		m_pChildren.insert(m_pChildren.begin() + m_pToBeAddedObject->m_IndexInHierarchy, m_pToBeAddedObject);
		m_pToBeAddedObject = nullptr;
	}
	else if (m_NeedChangeComponents)
	{
		auto temp = m_pComponents[m_ToBeChangedComponents.first];
		
		//erase from list
		m_pComponents.erase(m_pComponents.begin() + m_ToBeChangedComponents.first);

		// insert at correct position
		if (m_ToBeChangedComponents.second >= m_pComponents.size())
		{
			m_pComponents.push_back(temp);
		}
		else
		{
			m_pComponents.insert(m_pComponents.begin() + m_ToBeChangedComponents.second, temp);	
		}

		//Update index of elements
		unsigned int counter{ };
		std::for_each(m_pComponents.begin(), m_pComponents.end(), [counter](BaseComponent* pComponent) mutable
		{
			pComponent->SetIndexInHierarchy(counter);
			++counter;
		});

		m_NeedChangeComponents = false;
	}
}

void GameObject::LateUpdate()
{
	if (m_HasTransformChanged && m_pTransComp)
		m_pTransComp->UpdateTransform();

	for (auto child : m_pChildren)
		child->LateUpdate();
}

void GameObject::Render() const
{
	for (unsigned int i {m_pComponents.size()}; i > 0; i--)
	{
		m_pComponents[i - 1]->Render();
	}

	for (unsigned int i{ m_pChildren.size() }; i > 0; i--)
	{
		m_pChildren[i - 1]->Render();
	}
}

void GameObject::SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node)
{
	using namespace rapidxml;

	xml_node<>* objectNode = doc.allocate_node(node_element, "GameObject");
	objectNode->append_attribute(doc.allocate_attribute("Name", GetName().c_str()));
	node->append_node(objectNode);

	//Components
	//***********
	xml_node<>* componentsNode = doc.allocate_node(node_element, "Components");
	objectNode->append_node(componentsNode);
	if (m_pTransComp)
	{
		xml_node<>* compNode = doc.allocate_node(node_element, m_pTransComp->GetName().c_str());
		m_pTransComp->SaveAttributes(doc, compNode);
		componentsNode->append_node(compNode);
	}
	if (m_pComponents.size() > 0)
	{
		for (auto component : m_pComponents)
		{
			xml_node<>* compNode = doc.allocate_node(node_element, component->GetName().c_str());
			component->SaveAttributes(doc, compNode);
			componentsNode->append_node(compNode);
		}
	}	

	//Children
	//***********
	if (m_pChildren.size() > 0)
	{
		for (auto child : m_pChildren)
			child->SaveAttributes(doc, objectNode);
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
			SceneManager::GetInstance()->GetCurrentScene()->DetachChild(this);
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
		for (auto gameObject : m_pChildren) // CRASH HERE
		{
			gameObject->DrawInterfaceScene();
		}

		ImGui::TreePop();
	}

	//Space to drop an item in between 2 other items
	ImGui::Selectable("          ", false, 0, { 499, 0.5f });

	//Drop another object on this to change order of parent child
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			auto pObject = new GameObject(std::move(*(GameObject*)(payload->Data)));

			if (m_pParent)
				m_pParent->AddChild(pObject, this);
			else
				SceneManager::GetInstance()->GetCurrentScene()->AddChild(pObject, this);
		}

		ImGui::EndDragDropTarget();
	}
}

void GameObject::DrawInterfaceComponents()
{

	ImGui::Text("Text");
	ImGui::InputText("Text", &m_Name.front(), 128);

	if (m_pTransComp)
	{
		m_pTransComp->DrawInterface();
		if (ImGui::Button("Delete Component"))
		{
			delete m_pTransComp;
			m_pTransComp = nullptr;
		}
	}


	//List of components on gameobject
	auto it = m_pComponents.begin();
	while (it != m_pComponents.end())
	{	
		(*it)->DrawInterface();

		//Delete Component when asked
		ImGui::PushID((*it));
		if (ImGui::Button("Delete Component"))
		{
		
			it = m_pComponents.erase(it); 
		}
		else
			it++;
		ImGui::PopID();
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
			m_pTransComp = new TransformComponent(this);
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

	ImGui::Spacing();
	//Delete GameObject when asked
	if (!m_WantsToDeleteThis && ImGui::Button("Delete Object"))
	{
		m_WantsToDeleteThis = true;
		
	}
	//Confirmation
	if (m_WantsToDeleteThis)
	{
		if (ImGui::Button("Yes"))
		{
			if (m_pParent)
				m_pParent->DeleteChild(this);
			else
				m_pScene->DeleteChild(this);
		}
		ImGui::SameLine(0, 4);
		if (ImGui::Button("No"))
		{
			m_WantsToDeleteThis = false;
		}
	}
}

void GameObject::AddComponent(BaseComponent* pComponent)
{
	pComponent->SetIndexInHierarchy(m_pComponents.size());
	m_pComponents.emplace_back(pComponent);
}

void GameObject::AddChild(GameObject* pGameObject, GameObject* behindObject)
{
	if (behindObject == nullptr)
	{
		pGameObject->SetIndexInHierarchy(m_pChildren.size());
		m_pChildren.emplace_back(pGameObject);
	}
	else
	{
		auto it = std::find(m_pChildren.begin(), m_pChildren.end(), behindObject);
		pGameObject->SetIndexInHierarchy(std::distance(m_pChildren.begin(), it) + 1);
		m_pToBeAddedObject = pGameObject;
	}

	pGameObject->SetParent(this);
	pGameObject->SetScene(GetScene());
}

void GameObject::DetachChild(GameObject* pGameObject)
{
	//remove: It doesn’t actually delete elements from the container but only shunts non-deleted elements forwards on top of deleted elements.
	m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), pGameObject), m_pChildren.end());
	pGameObject->SetParent(nullptr);
}

void GameObject::DeleteChild(GameObject* pGameObject)
{
	m_pToBeDetachedChild = pGameObject;
	GameObjectManager::GetInstance()->SetSelectedGameObject(nullptr);
}

void GameObject::SetScene(Scene* pScene)
{
	m_pScene = pScene;
}

Scene* GameObject::GetScene()
{
	return m_pScene;;
}

void GameObject::SetParent(GameObject* pGameObject)
{
	m_pParent = pGameObject;
}

void GameObject::ChangeToFullScreen()
{
	auto transform = GetComponent<TransformComponent>();

	if (transform)
	{
		transform->SetPosition( SceneManager::GetInstance()->AdaptLocationToFullscreen( transform->GetPosition( ) ) );
		transform->SetScale( SceneManager::GetInstance()->AdaptScaleToFullscreen( transform->GetScale( ) ) );
	}
}

void GameObject::SetTransformChanged(bool changed)
{
	m_HasTransformChanged = changed;

	if (changed)
	{
		for (auto child : m_pChildren)
			child->SetTransformChanged(true);
	}
}

void GameObject::ChangeComponentOrder(BaseComponent* pBehindComp, unsigned int currentIndex)
{
	auto it = std::find(m_pComponents.begin(), m_pComponents.end(), pBehindComp);
	unsigned int index = std::distance(m_pComponents.begin(), it);

	if (currentIndex > index)
	{
		m_ToBeChangedComponents = { currentIndex, index + 1 };
	}
	else
	{
		m_ToBeChangedComponents = { currentIndex, index};
	}
	

	m_NeedChangeComponents = true;
}

