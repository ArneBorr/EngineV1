#include "MiniginPCH.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "imgui.h"
#include "BoxColliderComponent.h"

RigidbodyComponent::RigidbodyComponent(GameObject* pObject)
	: BaseComponent(pObject, "RigidbodyComponent")
{
	b2BodyDef bodyDef;
	Vector2f position = { 0, 0 };
	position = SceneManager::GetInstance()->AdapatPositionToView(position);
	bodyDef.position = { position.x, position.y };
	bodyDef.type = b2_dynamicBody;
	m_pBody = pObject->GetScene()->GetPhysicsWorld()->CreateBody(&bodyDef);

	auto boxCollider = pObject->GetComponent<BoxColliderComponent>();
	if (boxCollider)
	{
		boxCollider->CreateLink(this);
	}
}

RigidbodyComponent::~RigidbodyComponent()
{
	m_pBody = nullptr;
	if (m_pFicture)
	{
		m_pBody->DestroyFixture(m_pFicture);
		m_pFicture = nullptr;
	}
	m_pGameObject->GetScene()->GetPhysicsWorld()->DestroyBody(m_pBody);
}

void RigidbodyComponent::Render()
{
	//Renderer::GetInstance()->RenderTexture(*ResourceManager::GetInstance()->LoadTexture(" "), { m_pBody->GetTransform().p.x, m_pBody->GetTransform().p.y });
}

void RigidbodyComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void RigidbodyComponent::DrawInterface()
{
	using namespace ImGui;
	SetNextItemOpen(true, ImGuiCond_Once);

	bool open = TreeNode(&GetName().front());
	HandleDrag();

	if (open)
	{
		Separator();
		Spacing();

		Text("Rigidbody");

		

		TreePop();
	}

	HandleDrop();
}

void RigidbodyComponent::SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node)
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(node);
}

void RigidbodyComponent::ChangeShape(const b2PolygonShape& shape)
{
	if (m_pFicture)
		m_pBody->DestroyFixture(m_pFicture);

	b2FixtureDef fixtureDef;
	fixtureDef.density = m_Density;
	fixtureDef.friction = m_Friction;
	fixtureDef.shape = &shape;
	m_pFicture = m_pBody->CreateFixture(&fixtureDef);
}

void RigidbodyComponent::SetPosition(const Vector2f& pos)
{
	m_pBody->SetTransform({ pos.x, pos.y }, m_pBody->GetAngle());
}

Vector2f RigidbodyComponent::GetPosition() const
{
	const Vector2f pos{ m_pBody->GetPosition().x, m_pBody->GetPosition().y };
	return SceneManager::GetInstance()->ChangeToFullscreenCoord(pos);
}

void RigidbodyComponent::SetRotation(float rotation)
{
	m_pBody->SetTransform(m_pBody->GetPosition(), rotation);
}

