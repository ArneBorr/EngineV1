#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"

GameObject::~GameObject()
{
	for (unsigned int i{}; i < m_pBaseComponents.size(); i++)
	{
		delete m_pBaseComponents[i];
		m_pBaseComponents[i] = nullptr;
	}
}

void GameObject::Update(float elapsedSec)
{
	for (auto pComp : m_pBaseComponents)
	{
		pComp->Update(elapsedSec);
	}
}

void GameObject::Render() const
{
	for (auto pComp : m_pBaseComponents)
	{
		pComp->Render();
	}
}

void GameObject::AddComponent(BaseComponent* pComponent)
{
	m_pBaseComponents.push_back(pComponent);
}

TransformComponent GameObject::GetTransform()
{
	return m_Transform;
}
