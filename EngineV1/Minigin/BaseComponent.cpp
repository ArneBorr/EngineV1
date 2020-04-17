#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "GameObject.h"

BaseComponent::BaseComponent(GameObject* pGameObject, const std::string& name)
	: m_pGameObject{ pGameObject }
	, m_Name{ name }
{
}
