#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "GameObject.h"

BaseComponent::BaseComponent(std::weak_ptr<GameObject> pGameObject)
	: m_pGameObject{ pGameObject }
{
}
