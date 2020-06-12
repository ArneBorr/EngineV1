#include "MiniginPCH.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "imgui.h"
#include "BoxColliderComponent.h"
#include "TransformComponent.h"
#include "Subject.h"

RigidbodyComponent::RigidbodyComponent(GameObject* pObject)
	: BaseComponent(pObject, "RigidbodyComponent")
	, m_pSubject{ new Subject() }
{
	b2BodyDef bodyDef;
	auto transform{ pObject->GetTransform() };

	Vector2f position{};
	if (transform)
		position = transform->GetWorldPosition();
	else
		position = SceneManager::GetInstance()->AdapatPositionToEditor(position);

	bodyDef.position = { position.x / M_PPM, position.y / M_PPM };
	bodyDef.type = b2_staticBody;
	bodyDef.fixedRotation = m_HasFixedRotation;
	bodyDef.userData = this;
	m_pBody = pObject->GetScene()->GetPhysicsWorld()->CreateBody(&bodyDef);

	auto boxColliders = pObject->GetComponents<BoxColliderComponent>();
	for (auto boxCollider : boxColliders)
		boxCollider->CreateLink(this);
}

RigidbodyComponent::~RigidbodyComponent()
{
	m_pGameObject->GetScene()->GetPhysicsWorld()->DestroyBody(m_pBody);
	m_pBody = nullptr;

	delete m_pSubject;
	m_pSubject = nullptr;

	if (m_pGroundDetection)
	{
		delete m_pGroundDetection;
		m_pGroundDetection = nullptr;
	}
}

void RigidbodyComponent::Render()
{
	if (m_pGroundDetection)
		m_pGroundDetection->Render();
}

void RigidbodyComponent::Update(float elapsedSec)
{
	if (m_pGroundDetection)
		m_pGroundDetection->Update(elapsedSec);
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
			
		if (ImGui::RadioButton("Static", &m_TypeButtonIndex, 0))
			m_pBody->SetType(b2_staticBody);
		ImGui::SameLine();

		if (ImGui::RadioButton("Kinematic", &m_TypeButtonIndex, 1))
			m_pBody->SetType(b2_kinematicBody);

		if (ImGui::RadioButton("Dynamic", &m_TypeButtonIndex, 2))
			m_pBody->SetType(b2_dynamicBody);

		if (Checkbox("Fixed Rotation", &m_HasFixedRotation))
			m_pBody->SetFixedRotation(m_HasFixedRotation);

		if (Button("EnableroundDetector"))
			CreateGroundDetector();

		if (m_pGroundDetection)
			m_pGroundDetection->DrawInterface();

		TreePop();
	}

	HandleDrop();
}



void RigidbodyComponent::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("NoRotation", IntToXMLChar(doc, m_HasFixedRotation)));

	const char* typeString{};
	auto type = m_pBody->GetType();
	if (type == b2_staticBody)
		typeString = "Static";
	else if (type == b2_kinematicBody)
		typeString = "Kinematic";
	else if (type == b2_dynamicBody)
		typeString = "Dynamic";

	if (m_pGroundDetection)
	{
		rapidxml::xml_node<>* pGroundDetector = doc->allocate_node(rapidxml::node_element, "GroundDetector");
		m_pGroundDetection->SaveAttributes(doc, pGroundDetector);
		node->append_node(pGroundDetector);
	}

	node->append_attribute(doc->allocate_attribute("Type", typeString));
}

void RigidbodyComponent::SetAttributes(BoxColliderComponent* pGroundDetector, const std::string& type, bool fixedRot)
{
	m_pGameObject->SetRigidbody(this);

	m_HasFixedRotation = fixedRot;
	m_pBody->SetFixedRotation(fixedRot);
	m_pBody->SetUserData(this);
	m_pGroundDetection = pGroundDetector;
	if (m_pGroundDetection)
	{
		m_pGroundDetection->SetName("GroundDetector");
		m_pGroundDetection->CreateLink(this);
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

b2Fixture* RigidbodyComponent::AddShape(const b2FixtureDef& fictureDef)
{
	return m_pBody->CreateFixture(&fictureDef);
}

void RigidbodyComponent::DestroyShape(b2Fixture* ficture)
{
	m_pBody->DestroyFixture(ficture);
}

void RigidbodyComponent::EraseCollider(BoxColliderComponent* pComponent)
{
	if (m_pColliders.size() > 0)
		m_pColliders.erase(std::remove(m_pColliders.begin(), m_pColliders.end(), pComponent), m_pColliders.end());
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

Vector2f RigidbodyComponent::GetVelocity() const
{
	auto temp = m_pBody->GetLinearVelocity();
	return { temp.x, temp.y };
}

void RigidbodyComponent::SetVelocity(const Vector2f& vel)
{
	m_pBody->SetLinearVelocity({ vel.x, vel.y });
}

void RigidbodyComponent::SetIgnoreGroup(int group, bool ignore)
{
	for (auto collider : m_pColliders)
		collider->SetIgnoreGroup(group, ignore);
}

void RigidbodyComponent::AddCollider(BoxColliderComponent* pCollider)
{
	m_pColliders.push_back(pCollider);
}

void RigidbodyComponent::UpdateShapeScale()
{
	//Create new shape with scale automatically taken into account
	for (auto collider : m_pColliders)
		collider->CreateShape();
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

void RigidbodyComponent::LoadSettings(const std::string& settings)
{
	if (settings == "Player")
		LoadPlayerSettings();
	else if (settings == "Bubble")
		LoadBubbleSettings();
}

void RigidbodyComponent::CreateGroundDetector()
{
	m_pGroundDetection = new BoxColliderComponent(m_pGameObject);
	m_pGroundDetection->CreateLink(this);
	m_pGroundDetection->SetName("GroundDetector");
	m_pGroundDetection->SetAttributes({}, { }, 20, 20, 0, 0, 0, 0, 1, true);
}

void RigidbodyComponent::LoadPlayerSettings()
{
	m_pBody->SetType(b2_dynamicBody);
	m_TypeButtonIndex = 2;
	m_pBody->SetFixedRotation(true);
	m_HasFixedRotation = true;
}

void RigidbodyComponent::LoadBubbleSettings()
{
	m_pBody->SetType(b2_kinematicBody);
	m_TypeButtonIndex = 1;
	m_pBody->SetFixedRotation(true);
	m_HasFixedRotation = true;
}

void RigidbodyComponent::LoadZenChanSettings()
{
	m_pBody->SetType(b2_dynamicBody);
	m_TypeButtonIndex = 2;
	m_pBody->SetFixedRotation(true);
	m_HasFixedRotation = true;
}

