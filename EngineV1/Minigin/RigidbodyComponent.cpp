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
		position = SceneManager::GetInstance()->AdapatPositionToEditor(position);

	bodyDef.position = { position.x / M_PPM, position.y / M_PPM };
	bodyDef.type = b2_staticBody;
	m_pBody = pObject->GetScene()->GetPhysicsWorld()->CreateBody(&bodyDef);

	for (int i{}; i < m_NrOfCollGroups; i++)
	{
		m_CollisionItems[i] = std::to_string(i + 1);
		m_NotIgnoreGroups[i] = true;
	}

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
			PushItemWidth(50.f);
			if (BeginCombo("Collision Group", m_CollisionItems[m_SelectedCollGroupIndex].c_str()))
			{
				
				//Own collision group
				for (int i{}; i < m_NrOfCollGroups; i++)
				{
					const bool isSelected = m_SelectedCollGroupIndex == i;
					if (Selectable(m_CollisionItems[i].c_str(), isSelected))
					{
						m_SelectedCollGroupIndex = i;
						SetCollisionGroups();
					}

					if (isSelected)
						SetItemDefaultFocus();
				}
				EndCombo();
			}	

			PushItemWidth(50.f);
			//Set Ignore Groups
			if (BeginCombo("Ignore Group", NULL))
			{
				for (int i{}; i < m_NrOfCollGroups; i++)
				{
					if (Selectable(m_CollisionItems[i].c_str(), !m_NotIgnoreGroups[i]))
					{
						m_NotIgnoreGroups[i] = !m_NotIgnoreGroups[i];
						SetCollisionGroups();
					}

					if (m_NotIgnoreGroups[i])
						SetItemDefaultFocus();
				}

				EndCombo();
			}
			
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

		if (Checkbox("Fixed Rotation", &m_HasFixedRotation))
			m_pBody->SetFixedRotation(m_HasFixedRotation);

		TreePop();
	}

	HandleDrop();
}



void RigidbodyComponent::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	int group{};
	switch (m_CollisionGroup)
	{
	case CollisionGroup::One:
		group = 1;
		break;
	case CollisionGroup::Two:
		group = 2;
		break;
	case CollisionGroup::Three:
		group = 3;
		break;
	case CollisionGroup::Four:
		group = 4;
		break;
	case CollisionGroup::Five:
		group = 5;
		break;
	}

	node->append_attribute(doc->allocate_attribute("CollGroup",IntToXMLChar(doc, group)));

	//Was not possible with loop, xml would not be able to read number of loop (i)
	node->append_attribute(doc->allocate_attribute("IgnoreGr0", IntToXMLChar(doc, !m_NotIgnoreGroups[0])));
	node->append_attribute(doc->allocate_attribute("IgnoreGr1", IntToXMLChar(doc, !m_NotIgnoreGroups[1])));
	node->append_attribute(doc->allocate_attribute("IgnoreGr2", IntToXMLChar(doc, !m_NotIgnoreGroups[2])));
	node->append_attribute(doc->allocate_attribute("IgnoreGr3", IntToXMLChar(doc, !m_NotIgnoreGroups[3])));
	node->append_attribute(doc->allocate_attribute("IgnoreGr4", IntToXMLChar(doc, !m_NotIgnoreGroups[4])));	

	node->append_attribute(doc->allocate_attribute("Density", FloatToXMLChar(doc, m_Density)));
	node->append_attribute(doc->allocate_attribute("Friction", FloatToXMLChar(doc, m_Friction)));
	node->append_attribute(doc->allocate_attribute("Restitution", FloatToXMLChar(doc, m_Restitution)));
	node->append_attribute(doc->allocate_attribute("NoRotation", IntToXMLChar(doc, m_HasFixedRotation)));

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

void RigidbodyComponent::SetAttributes(const std::vector<bool>& ignoreGroups, const std::string& type, float density, float friction, float restitution, int collGroup, bool fixedRot)
{
	m_pGameObject->SetRigidbody(this);

	m_Density = density;
	m_Friction = friction;
	m_Restitution = restitution;
	m_CollisionGroup = GetCollGroup(collGroup - 1);
	m_SelectedCollGroupIndex = collGroup - 1;
	m_HasFixedRotation = fixedRot;

	for (unsigned int i{}; i < ignoreGroups.size(); i++)
		m_NotIgnoreGroups[i] = !ignoreGroups[i];

	if (m_pFicture)
	{
		m_pFicture->SetDensity(density);
		m_pFicture->SetFriction(friction);
		m_pFicture->SetRestitution(restitution);
		m_pFicture->SetFilterData(GetFilter());
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

	b2FixtureDef fictureDef;
	fictureDef.density = m_Density;
	fictureDef.friction = m_Friction;
	fictureDef.restitution = m_Restitution;
	fictureDef.shape = &shape;
	fictureDef.userData = pBox;

	fictureDef.filter = GetFilter();
	m_pFicture = m_pBody->CreateFixture(&fictureDef);
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

void RigidbodyComponent::SetIgnoreGroups(std::vector<bool> ignoreGroups)
{
	if (ignoreGroups.size() != m_NrOfCollGroups)
		return; // Logger

	for (int i{}; i < m_NrOfCollGroups; i++)
	{
		m_NotIgnoreGroups[i] = !ignoreGroups[i];
	}

	SetCollisionGroups();
}

void RigidbodyComponent::SetIgnoreGroup(int i, bool ignore)
{
	if (i > 1 && i <= m_NrOfCollGroups)
		m_NotIgnoreGroups[i - 1] = !ignore;

	SetCollisionGroups();
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

void RigidbodyComponent::SetCollisionGroups()
{
	m_pFicture->SetFilterData(GetFilter());
}

RigidbodyComponent::CollisionGroup RigidbodyComponent::GetCollGroup(int i)
{
	switch (i)
	{
	case 0:
		return CollisionGroup::One;
		break;
	case 1:
		return CollisionGroup::Two;
		break;
	case 2:
		return CollisionGroup::Three;
		break;
	case 3:
		return CollisionGroup::Four;
		break;
	case 4:
		return CollisionGroup::Five;
		break;
	}

	return  CollisionGroup::One;
}

b2Filter RigidbodyComponent::GetFilter()
{
	uint16 maskBits{ None };

	for (int i{}; i < m_NrOfCollGroups; i++)
	{
		if (m_NotIgnoreGroups[i])
		{
			maskBits |= GetCollGroup(i);
		}
	}

	m_CollisionGroup = GetCollGroup(m_SelectedCollGroupIndex);

	b2Filter filter;
	filter.categoryBits = m_CollisionGroup;
	filter.maskBits = maskBits;

	return filter;
}

