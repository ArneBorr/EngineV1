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

	bodyDef.position = { position.x / M_PPM, position.y / M_PPM };
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
			if (InputFloat("Restitution", &m_Restitution))
				m_pFicture->SetRestitution(m_Restitution);
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

void RigidbodyComponent::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Density", FloatToXMLChar(doc, m_Density)));
	node->append_attribute(doc->allocate_attribute("Friction", FloatToXMLChar(doc, m_Friction)));
	node->append_attribute(doc->allocate_attribute("Restitution", FloatToXMLChar(doc, m_Restitution)));

	const char* typeString{};
	auto type = m_pBody->GetType();
	if (type == b2_staticBody)
		typeString = "Static";
	else if (type == b2_kinematicBody)
		typeString = "Kinematic";
	else if (type == b2_dynamicBody)
		typeString = "Dynamic";

	node->append_attribute(doc->allocate_attribute("Type", typeString));
}

void RigidbodyComponent::SetAttributes(const std::string& type, float density, float friction, float restitution)
{
	m_pGameObject->SetRigidbody(this);

	m_Density = density;
	m_Friction = friction;
	m_Restitution = restitution;

	if (m_pFicture)
	{
		m_pFicture->SetDensity(density);
		m_pFicture->SetFriction(friction);
		m_pFicture->SetRestitution(restitution);
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
	fixtureDef.restitution = m_Restitution;
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
	return Vector2f{ m_pBody->GetPosition().x * M_PPM, m_pBody->GetPosition().y * M_PPM };
}

void RigidbodyComponent::SetRotation(float rotation)
{
	m_pBody->SetTransform(m_pBody->GetPosition(), rotation * M_PI / 180.f);
}

Vector2f RigidbodyComponent::GetVelocity()
{
	auto temp = m_pBody->GetLinearVelocity();
	return { temp.x, temp.y };
}

void RigidbodyComponent::UpdateShapeScale()
{
	//Create new shape with scale automatically taken into account
	if (m_pFicture)
		static_cast<BoxColliderComponent*>(m_pFicture->GetUserData())->CreateLink(this);;
}

void RigidbodyComponent::Move(const Vector2f& vel, const Vector2f& maxVel)
{
	auto bodyVel = m_pBody->GetLinearVelocity();
	bodyVel.x += vel.x * M_PPM;
	bodyVel.y += vel.y * M_PPM;

	Clamp(bodyVel.x, -maxVel.x, maxVel.x);
	Clamp(bodyVel.y, -maxVel.y, maxVel.y);

	m_pBody->SetLinearVelocity( bodyVel );
}

void RigidbodyComponent::Jump(float strength)
{
	m_pBody->ApplyForce({ 0, -strength * M_PPM }, { m_pBody->GetPosition().x,m_pBody->GetPosition().y - 4.f }, true);
}

