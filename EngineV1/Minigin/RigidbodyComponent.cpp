#include "MiniginPCH.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "imgui.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"

RigidbodyComponent::RigidbodyComponent(GameObject* pObject)
	: BaseComponent(pObject, "RigidbodyComponent")
{
	b2BodyDef bodyDef;
	auto transform{ pObject->GetTransform() };

	Vector2f position{};
	if (transform)
		position = transform->GetWorldPosition();
	else
		position = SceneManager::GetInstance()->AdapatPositionToView(position);

	bodyDef.position = { position.x, position.y };
	bodyDef.type = b2_staticBody;
	m_pBody = pObject->GetScene()->GetPhysicsWorld()->CreateBody(&bodyDef);

	auto boxCollider = pObject->GetComponent<BoxColliderComponent>();
	if (boxCollider)
	{
		boxCollider->CreateLink(this);
	}
}

RigidbodyComponent::~RigidbodyComponent()
{
	if (m_pFicture)
	{
		m_pBody->DestroyFixture(m_pFicture);
		m_pFicture = nullptr;
	}

	m_pGameObject->GetScene()->GetPhysicsWorld()->DestroyBody(m_pBody);
	m_pBody = nullptr;
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

		if (m_pFicture)
		{
			if (InputFloat("Density", &m_Density))
				m_pFicture->SetDensity(m_Density);
			if (InputFloat("Friction", &m_Friction))
				m_pFicture->SetFriction(m_Friction);
		}
			
		if (ImGui::RadioButton("Static", &m_TypeButtonIndex, 0))
			m_pBody->SetType(b2_staticBody);
		ImGui::SameLine();

		if (ImGui::RadioButton("Kinematic", &m_TypeButtonIndex, 1))
			m_pBody->SetType(b2_kinematicBody);

		if (ImGui::RadioButton("Dynamic", &m_TypeButtonIndex, 2))
			m_pBody->SetType(b2_dynamicBody);

		TreePop();
	}

	HandleDrop();
}

void RigidbodyComponent::SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc.allocate_attribute("Density", FloatToXMLChar(doc, m_Density)));
	node->append_attribute(doc.allocate_attribute("Friction", FloatToXMLChar(doc, m_Friction)));

	const char* typeString{};
	auto type = m_pBody->GetType();
	if (type == b2_staticBody)
		typeString = "Static";
	else if (type == b2_kinematicBody)
		typeString = "Kinematic";
	else if (type == b2_dynamicBody)
		typeString = "Dynamic";

	node->append_attribute(doc.allocate_attribute("Type", typeString));
}

void RigidbodyComponent::SetAttributes(const std::string& type, float density, float friction)
{
	m_pGameObject->SetRigidbody(this);

	if (m_pFicture)
	{
		m_pFicture->SetDensity(density);
		m_pFicture->SetFriction(friction);
	}

	if (type == "Static")
	{
		m_pBody->SetType(b2_staticBody);
		m_TypeButtonIndex = 0;
	}
	else if (type == "Kinematic")
	{
		m_pBody->SetType(b2_kinematicBody);
		m_TypeButtonIndex = 1;
	}
	else if (type == "Dynamic")
	{
		m_pBody->SetType(b2_dynamicBody);
		m_TypeButtonIndex = 2;
	}
}

void RigidbodyComponent::ChangeShape(BoxColliderComponent* pBox, const b2PolygonShape& shape)
{
	if (m_pFicture)
		m_pBody->DestroyFixture(m_pFicture);

	b2FixtureDef fixtureDef;
	fixtureDef.density = m_Density;
	fixtureDef.friction = m_Friction;
	fixtureDef.shape = &shape;
	fixtureDef.userData = pBox;
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

void RigidbodyComponent::UpdateShapeScale()
{
	//Create new shape with scale taken into account
	if (m_pFicture)
		static_cast<BoxColliderComponent*>(m_pFicture->GetUserData())->CreateLink(this);;
}

