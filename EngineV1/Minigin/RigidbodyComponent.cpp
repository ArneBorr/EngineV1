#include "MiniginPCH.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "Scene.h"

RigidbodyComponent::RigidbodyComponent(GameObject* pObject, const std::string& name)
	: BaseComponent(pObject, name)
{
	b2BodyDef bodyDef;
	bodyDef.position = { 0, 0 };
	m_pBody = pObject->GetScene()->GetPhysicsWorld()->CreateBody(&bodyDef);
}

RigidbodyComponent::~RigidbodyComponent()
{
	m_pBody = nullptr;
	m_pGameObject->GetScene()->GetPhysicsWorld()->DestroyBody(m_pBody);
}

void RigidbodyComponent::Render()
{

}

void RigidbodyComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void RigidbodyComponent::DrawInterface()
{
}

void RigidbodyComponent::SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node)
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(node);
}
