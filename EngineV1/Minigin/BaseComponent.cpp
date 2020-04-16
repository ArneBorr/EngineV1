#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "GameObject.h"

BaseComponent::BaseComponent(GameObject* pGameObject)
	: m_pGameObject{ pGameObject }
{
}
