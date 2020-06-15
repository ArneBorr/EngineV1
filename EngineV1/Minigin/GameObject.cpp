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
	
{
	strcpy_s(m_Name, name.c_str());
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

	if (m_pTransform)
	{
		delete m_pTransform;
		m_pTransform = nullptr;
	}

	m_pScene = nullptr;
	m_pParent = nullptr;

	delete m_pToBeAddedObject;
	m_pToBeAddedObject = nullptr;
	delete m_pToBeDetachedChild;
	m_pToBeDetachedChild = nullptr;	
}

GameObject::GameObject(GameObject&& other) noexcept
{
	if (this != &other)
	{
		for (auto child : m_pChildren)
		{
			delete child;
			child = nullptr;
		}
		m_pChildren.clear();
		m_pChildren = other.m_pChildren;
		for (auto child : other.m_pChildren)
			child = nullptr;
		other.m_pChildren.clear();

		m_pScene = other.m_pScene;
		other.m_pScene = nullptr;

		delete m_pToBeAddedObject;
		m_pToBeAddedObject = other.m_pToBeAddedObject;
		other.m_pToBeAddedObject = nullptr;

		delete m_pToBeDetachedChild;
		m_pToBeDetachedChild = other.m_pToBeDetachedChild;
		other.m_pToBeDetachedChild = nullptr;

		memset(m_Name, 0, sizeof(m_Name));
		memcpy(m_Name, other.m_Name, sizeof(m_Name));
		memset(other.m_Name, 0, sizeof(m_Name));

		m_ToBeChangedComponents = other.m_ToBeChangedComponents;

		m_pParent = other.m_pParent;
		other.m_pParent = nullptr;

		for (auto comp : m_pComponents)
		{
			delete comp;
			comp = nullptr;
		}
		m_pComponents.clear();
		m_pComponents = other.m_pComponents;
		other.m_pComponents.clear();

		for (auto comp : m_pComponents)
			comp->SetGameObject(this);

		delete m_pRigidbody;
		m_pRigidbody = other.m_pRigidbody;
		other.m_pRigidbody = nullptr;
		if (m_pRigidbody)
			m_pRigidbody->SetGameObject(this);

		delete m_pTransform;
		m_pTransform = other.m_pTransform;
		other.m_pTransform = nullptr;
		if (m_pTransform)
			m_pTransform->SetGameObject(this);
	}
}

void GameObject::Initialize()
{
	if (m_pTransform)
		m_pTransform->Initialize();

	for (auto pComponent : m_pComponents)
		pComponent->Initialize();
}

void GameObject::LateInitialize()
{
	for (auto pComponent : m_pComponents)
		pComponent->LateInitialize();
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

	for (auto pChild : m_pChildren)
	{
		pChild->Update(elapsedSec);
	}
	
	ChangeHierarchy();
}

void GameObject::LateUpdate()
{
	if (m_pTransform)
	{
		//When transform is changed, recalculate + adapt rigidbody (which is also in UpdateTransform())
		if (m_HasTransformChanged)
		{	
			m_pTransform->UpdateTransform(true);
		}
		//Adapt transform to rigidbody
		else if (m_pRigidbody)
		{
			m_pTransform->SetPosition(m_pRigidbody->GetPosition());
			m_pTransform->SetRotation(m_pRigidbody->GetRotation());
			m_pTransform->UpdateTransform(false);
		}
	}
	

	for (auto child : m_pChildren)
		child->LateUpdate();
}

void GameObject::Render() const
{
	//Render in reverse order so first child.component is render on top
	for (size_t i {m_pComponents.size()}; i > 0; i--)
	{
		m_pComponents[i - 1]->Render();
	}

	for (size_t i{ m_pChildren.size() }; i > 0; i--)
	{
		m_pChildren[i - 1]->Render();
	}
}

void GameObject::Reset()
{
	if (m_pTransform)
		m_pTransform->Reset();
}

void GameObject::SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode)
{
	using namespace rapidxml;

	xml_node<>* objectNode = pDoc->allocate_node(node_element, "GameObject");
	objectNode->append_attribute(pDoc->allocate_attribute("Name", GetName()));
	pNode->append_node(objectNode);

	//Tags
	//***********
	xml_node<>* tagNode = pDoc->allocate_node(node_element, "Tags");
	for (const auto& tag : m_Tags)
		tagNode->append_attribute(pDoc->allocate_attribute("Tag", tag.c_str()));
	objectNode->append_node(tagNode);

	//Components
	//***********
	xml_node<>* componentsNode = pDoc->allocate_node(node_element, "Components");
	objectNode->append_node(componentsNode);
	if (m_pTransform)
	{
		xml_node<>* compNode = pDoc->allocate_node(node_element, m_pTransform->GetName().c_str());
		m_pTransform->SaveAttributes(pDoc, compNode);
		componentsNode->append_node(compNode);
	}
	if (m_pComponents.size() > 0)
	{
		for (auto component : m_pComponents)
		{
			xml_node<>* compNode = pDoc->allocate_node(node_element, component->GetName().c_str());
			component->SaveAttributes(pDoc, compNode);
			componentsNode->append_node(compNode);
		}
	}	

	//Children
	//***********
	if (m_pChildren.size() > 0)
	{
		for (auto child : m_pChildren)
			child->SaveAttributes(pDoc, objectNode);
	}
}

void GameObject::DrawInterfaceScene()
{
	using namespace ImGui;

	SetNextItemOpen(true, ImGuiCond_Once);
	bool open = TreeNode(GetName());

	// Drag this object to change the parent
	if (BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		SetDragDropPayload("GameObject", nullptr, 0, ImGuiCond_Once);
		Text(GetName());  // Display when moving
		EndDragDropSource();
	}

	//Drop another object on this to make the object a child
	if (BeginDragDropTarget())
	{
		if (const ImGuiPayload* pPayload = AcceptDragDropPayload("GameObject"))
		{
			auto pGameObject = GameObjectManager::GetInstance()->GetSelectedGameObject();
			pGameObject->DetachThis();
			AddChild(pGameObject);
		}

		EndDragDropTarget();
	}
	
	//Show components of this game object when selected
	if (IsItemClicked())
		GameObjectManager::GetInstance()->SetSelectedGameObject(this);

	//Space to drop an item in between 2 other items
	Selectable("          ", false, 0, { 499, 0.5f });

	//Drop another object on this to change order of parent child
	if (BeginDragDropTarget())
	{
		if (const ImGuiPayload* pPayload = AcceptDragDropPayload("GameObject"))
		{
			auto pGameObject = GameObjectManager::GetInstance()->GetSelectedGameObject();
			pGameObject->DetachThis();
			if (m_pParent)
				m_pParent->AddChild(pGameObject, this);
			else
				SceneManager::GetInstance()->GetCurrentScene()->AddChild(pGameObject, this);
		}

		EndDragDropTarget();
	}

	//Show childs
	if (open)
	{
		for (auto pGameObject : m_pChildren)
		{
			pGameObject->DrawInterfaceScene();
		}

		TreePop();
	}
}

void GameObject::DrawInterfaceComponents()
{
	using namespace ImGui;

	Text("Name");
	InputText("Text", m_Name, 128);
	Separator();

	//Draw tags
	for (auto it = m_Tags.begin(); it != m_Tags.end();)
	{
		PushID(&it);
		Text((*it).c_str());
		SameLine();
		if (Button("Remove"))
			it = m_Tags.erase(it);
		else
			++it;

		PopID();
	}

	//Add tag
	static char m_TagText[30]{"NewTag"};
	InputText("  ", m_TagText, IM_ARRAYSIZE(m_TagText));
	SameLine();
	if (Button("Tag"))
		m_Tags.push_back(m_TagText);

	//Draw transform
	if (m_pTransform)
	{
		m_pTransform->DrawInterface();
		if (Button("Delete Component"))
		{
			delete m_pTransform;
			m_pTransform = nullptr;
		}
	}

	//List of components on gameobject
	auto it = m_pComponents.begin();
	while (it != m_pComponents.end())
	{	
		PushID(*it);
		(*it)->DrawInterface();


		//Delete Component when asked
		if (Button("Delete Component"))
		{
			if ((*it) == m_pRigidbody)
				m_pRigidbody = nullptr;

			delete (*it);
			(*it) = nullptr;
			it = m_pComponents.erase(it); 
		}
		else
			it++;

		PopID();
	}

	//List of components that you can add
	//Do this cleaner?
	static const char* PossibleComponents[] = { "TransformComponent", "TextureComponent", "TextComponent", "RigidbodyComponent", "BoxColliderComponent", "MovementComponent", "AnimationController"
	, "ScriptComponent", "FSMComponent"};

	static int currentAddableCompIndex = 0;
	Separator();
	Combo(" ", &currentAddableCompIndex, PossibleComponents, IM_ARRAYSIZE(PossibleComponents));
	SameLine();

	//if clicked, add component that is selected
	if (Button("Add"))
	{
		BaseComponent* pComponent = nullptr;
		std::string item = PossibleComponents[currentAddableCompIndex];

		if (item == "TransformComponent")
		{
			if (!m_pTransform)
				m_pTransform = new TransformComponent(this);
		}
		else if (item == "TextureComponent")
		{
			pComponent = new TextureComponent(this, "logo.png");
		}
		else if (item == "TextComponent")
		{
			pComponent = new TextComponent(this);
		}
		else if (item == "RigidbodyComponent")
		{
			if (!m_pRigidbody)
			{
				m_pRigidbody = new RigidbodyComponent(this);
				pComponent = m_pRigidbody;
			}		
		}
		else if (item == "BoxColliderComponent")
		{
			pComponent = new BoxColliderComponent(this);
		}
		else if (item == "MovementComponent")
		{
			pComponent = new MovementComponent(this);
		}
		else if (item == "AnimationController")
		{
			pComponent = new AnimatorControllerComponent(this);
		}
		else if (item == "ScriptComponent")
		{
			pComponent = new ScriptComponent(this);
		}
		else if (item == "FSMComponent")
		{
			pComponent = new FSMComponent(this);
		}

		if (pComponent)
			AddComponent(pComponent);
	}

	Spacing();
	//Delete GameObject when asked
	if (!m_WantsToDeleteThis && Button("Delete Object"))
	{
		m_WantsToDeleteThis = true;
	}
	//Confirmation
	if (m_WantsToDeleteThis)
	{
		if (Button("Yes"))
		{
			if (m_pParent)
				m_pParent->DeleteChild(this);
			else
				m_pScene->DeleteChild(this);
		}
		SameLine(0, 4);
		if (ImGui::Button("No"))
		{
			m_WantsToDeleteThis = false;
		}
	}
}

void GameObject::AddComponent(BaseComponent* pComponent)
{
	pComponent->SetIndexInHierarchy(UINT(m_pComponents.size()));
	m_pComponents.emplace_back(pComponent);
}

void GameObject::AddChild(GameObject* pGameObject, GameObject* pBehindObject)
{
	//Just place it at the end
	if (pBehindObject == nullptr)
	{
		pGameObject->SetIndexInHierarchy(UINT(m_pChildren.size()));
		m_pChildren.emplace_back(pGameObject);
	}
	//Place it behind a child
	else
	{
		auto it = std::find(m_pChildren.begin(), m_pChildren.end(), pBehindObject);
		pGameObject->SetIndexInHierarchy(UINT(std::distance(m_pChildren.begin(), it) + 1));
		m_pToBeAddedObject = pGameObject;
	}

	pGameObject->SetParent(this);
	pGameObject->SetScene(GetScene());
}

void GameObject::DetachChild(GameObject* pGameObject)
{
	m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), pGameObject), m_pChildren.end());
	pGameObject->SetParent(nullptr);
}

void GameObject::DeleteChild(GameObject* pGameObject)
{
	m_pToBeDetachedChild = pGameObject;
	GameObjectManager::GetInstance()->SetSelectedGameObject(nullptr);
}

void GameObject::DetachThis()
{
	if (m_pParent)
	{
		m_pParent->DetachChild(this);
	}
	else
	{
		m_pScene->DetachChild(this);
	}
}

void GameObject::SetScene(Scene* pScene)
{
	m_pScene = pScene;
}

Scene* GameObject::GetScene() const
{
	return m_pScene;;
}

void GameObject::SetParent(GameObject* pGameObject)
{
	m_pParent = pGameObject;
}

void GameObject::AdaptToFullScreen(const Vector2f& ratio)
{
	if (m_pTransform)
	{
		m_pTransform->SetPosition( SceneManager::GetInstance()->ChangeToFullscreenCoord(m_pTransform->GetWorldPosition( ) ) );
		m_pTransform->SetScale(SceneManager::GetInstance()->AdaptScaleToFullscreen(m_pTransform->GetWorldScale()));
		m_pTransform->UpdateTransform(true);
	}

	for (auto component : m_pComponents)
		component->AdaptToFullscreen(ratio);

	for (auto child : m_pChildren)
		child->AdaptToFullScreen(ratio);
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

void GameObject::AddTag(const std::string& tag)
{
	m_Tags.push_back(tag);
}

bool GameObject::HasTags(const std::vector<std::string>& tags)
{
	for (const auto& tag : m_Tags)
	{
		for (const auto& wantedTag : tags)
		{
			if (wantedTag == tag)
				return true;
		}
	}

	return false;
}

void GameObject::ChangeHierarchy()
{
	// Change hierachy when needed
	if (m_pToBeAddedObject)
	{
		m_pChildren.insert(m_pChildren.begin() + m_pToBeAddedObject->m_IndexInHierarchy, m_pToBeAddedObject);
		m_pToBeAddedObject = nullptr;
	}
	//Change hierarchy of components
	else if (m_NeedChangeComponents)
	{
		auto temp = m_pComponents[m_ToBeChangedComponents.first];

		//erase from list
		m_pComponents.erase(m_pComponents.begin() + m_ToBeChangedComponents.first);

		// insert at new correct position
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

void GameObject::ChangeComponentOrder(BaseComponent* pBehindComp, unsigned int currentIndex)
{
	auto it = std::find(m_pComponents.begin(), m_pComponents.end(), pBehindComp);
	unsigned int index = UINT(std::distance(m_pComponents.begin(), it));

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

