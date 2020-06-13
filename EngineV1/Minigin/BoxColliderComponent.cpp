#include "MiniginPCH.h"
#include "BoxColliderComponent.h"
#include "imgui.h"
#include "RigidbodyComponent.h"
#include "Renderer.h"
#include <SDL.h>
#include "TransformComponent.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "Subject.h"

BoxColliderComponent::BoxColliderComponent(GameObject* pObject)
	: BaseComponent(pObject, "BoxColliderComponent")
{
	for (int i{}; i < m_NrOfCollGroups; i++)
	{
		m_CollisionItems[i] = std::to_string(i + 1);
		m_IgnoreGroups[i] = false;
	}

	m_pRigidbody = pObject->GetRigidbody();
	if (m_pRigidbody)
	{
		CreateShape();
		m_pRigidbody->AddCollider(this);
	}
	
	m_pTexture = ResourceManager::GetInstance()->LoadTexture("BoxOutline.png");
}

BoxColliderComponent::~BoxColliderComponent()
{
	if (m_pRigidbody)
	{
		m_pRigidbody->EraseCollider(this);
		m_pRigidbody->DestroyShape(m_pFicture);
	}

	delete m_pTexture;
	m_pTexture = nullptr;
}

void BoxColliderComponent::Render()
{
	if (!m_RenderCollider)
		return;

	auto transform = m_pGameObject->GetTransform();
	if (!transform)
		return;

	Vector2f pos{};
	if (m_pRigidbody)
		pos = m_pRigidbody->GetPosition();
	else
		pos = transform->GetWorldPosition();

	const Vector2f scale = transform->GetWorldScale();
	const float scaleX = m_Width / 300.f * scale.x; // Texture width = 300;
	const float scaleY = m_Height / 300.f * scale.y; // Texture width = 300;
	if (m_pRigidbody)
		Renderer::GetInstance()->RenderTexture(*m_pTexture, { pos.x + m_Offset.x, pos.y + m_Offset.y }, {}, { scaleX, scaleY }, m_pRigidbody->GetRotation(), true);
	else
		Renderer::GetInstance()->RenderTexture(*m_pTexture, { pos.x + m_Offset.x, pos.y + m_Offset.y }, {}, { scaleX, scaleY }, transform->GetWorldRotation(), true);
}

void BoxColliderComponent::Update(float)
{
}

void BoxColliderComponent::DrawInterface()
{
	using namespace ImGui;
	SetNextItemOpen(true, ImGuiCond_Once);

	bool open = TreeNode(&GetName().front());
	HandleDrag();

	if (open)
	{
		Separator();
		Spacing();

		Text("BoxCollider");

		PushItemWidth(150.f);
		if (InputFloat("Width", &m_Width, 1.f, 50.f, "%.1f"))
			CreateShape();
		PushItemWidth(150.f);
		if (InputFloat("Height", &m_Height, 1.f, 50.f, "%.1f"))
			CreateShape();

		Text("Offset");
		PushItemWidth(150.f);
		InputFloat("x", &m_Offset.x, 1.f, 50.f, "%.1f");
		PushItemWidth(150.f);
		InputFloat("y", &m_Offset.y, 1.f, 50.f, "%.1f");
		Checkbox("Render Collider", &m_RenderCollider);

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
					if (Selectable(m_CollisionItems[i].c_str(), m_IgnoreGroups[i]))
					{
						m_IgnoreGroups[i] = !m_IgnoreGroups[i];
						SetCollisionGroups();
					}

					if (m_IgnoreGroups[i])
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

			if (Checkbox("Sensor", &m_IsSensor))
				m_pFicture->SetSensor(m_IsSensor);
		}
		TreePop();
	}

	HandleDrop();
}

void BoxColliderComponent::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Width", FloatToXMLChar(doc, m_Width)));
	node->append_attribute(doc->allocate_attribute("Height", FloatToXMLChar(doc, m_Height)));
	node->append_attribute(doc->allocate_attribute("OffsetX", FloatToXMLChar(doc, m_Offset.x)));
	node->append_attribute(doc->allocate_attribute("OffsetY", FloatToXMLChar(doc, m_Offset.y)));
	node->append_attribute(doc->allocate_attribute("RenderCollider", IntToXMLChar(doc, m_RenderCollider)));

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

	node->append_attribute(doc->allocate_attribute("CollGroup", IntToXMLChar(doc, group)));

	//Was not possible with loop, xml was able to read number of loop (i)
	node->append_attribute(doc->allocate_attribute("IgnoreGr0", IntToXMLChar(doc, m_IgnoreGroups[0])));
	node->append_attribute(doc->allocate_attribute("IgnoreGr1", IntToXMLChar(doc, m_IgnoreGroups[1])));
	node->append_attribute(doc->allocate_attribute("IgnoreGr2", IntToXMLChar(doc, m_IgnoreGroups[2])));
	node->append_attribute(doc->allocate_attribute("IgnoreGr3", IntToXMLChar(doc, m_IgnoreGroups[3])));
	node->append_attribute(doc->allocate_attribute("IgnoreGr4", IntToXMLChar(doc, m_IgnoreGroups[4])));

	node->append_attribute(doc->allocate_attribute("Density", FloatToXMLChar(doc, m_Density)));
	node->append_attribute(doc->allocate_attribute("Friction", FloatToXMLChar(doc, m_Friction)));
	node->append_attribute(doc->allocate_attribute("Restitution", FloatToXMLChar(doc, m_Restitution)));
	node->append_attribute(doc->allocate_attribute("IsSensor", IntToXMLChar(doc, m_IsSensor)));

	CreateShape();
}

void BoxColliderComponent::SetAttributes(const std::vector<bool>& ignoreGroups, const Vector2f& offset, 
	float width, float height, float density, float friction, float restitution, int collGroup, int renderCollider, int isSensor)
{
	m_Width = width;
	m_Height = height;
	m_Offset = offset;
	m_RenderCollider = renderCollider;

	m_Density = density;
	m_Friction = friction;
	m_Restitution = restitution;
	m_CollisionGroup = GetCollGroup(collGroup - 1);
	m_SelectedCollGroupIndex = collGroup - 1;
	m_IsSensor = isSensor;

	for (unsigned int i{}; i < ignoreGroups.size(); i++)
		m_IgnoreGroups[i] = ignoreGroups[i];

	if (m_pFicture)
	{
		m_pFicture->SetDensity(density);
		m_pFicture->SetFriction(friction);
		m_pFicture->SetRestitution(restitution);
		m_pFicture->SetFilterData(GetFilter());
		m_pFicture->SetSensor(m_IsSensor);
	}

	CreateShape();
}

void BoxColliderComponent::SetIgnoreGroups(const std::vector<bool>& ignoreGroups)
{
	if (ignoreGroups.size() != m_NrOfCollGroups)
		return; // Logger

	for (int i{}; i < m_NrOfCollGroups; i++)
	{
		m_IgnoreGroups[i] = ignoreGroups[i];
	}

	SetCollisionGroups();
}

void BoxColliderComponent::SetIgnoreGroup(int i, bool ignore)
{
	if (i > 0 && i <= m_NrOfCollGroups)
		m_IgnoreGroups[i - 1] = ignore;

	SetCollisionGroups();
}

void BoxColliderComponent::CreateLink(RigidbodyComponent* pBody)
{
	m_pRigidbody = pBody;
	m_pRigidbody->AddCollider(this);
	CreateShape();
}

b2Fixture* BoxColliderComponent::Unlink()
{
	m_pRigidbody = nullptr;
	auto temp = m_pFicture;
	m_pFicture = nullptr;
	return temp;
}

BoxColliderComponent::CollisionGroup BoxColliderComponent::GetCollGroup(int i)
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

b2Filter BoxColliderComponent::GetFilter()
{
	uint16 maskBits{ None };

	for (int i{}; i < m_NrOfCollGroups; i++)
	{
		if (!m_IgnoreGroups[i])
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

void BoxColliderComponent::SetCollisionGroups()
{
	m_pFicture->SetFilterData(GetFilter());
}

void BoxColliderComponent::CreateShape()
{
	Vector2f scale{ 1,1 };
	const auto pTranform = m_pGameObject->GetTransform();
	if (pTranform)
		scale = pTranform->GetWorldScale();
	
	if ( m_pRigidbody && m_Width != 0 && m_Height != 0 && scale.x != 0 && scale.y != 0)
	{		
		b2FixtureDef fixtureDef;
		fixtureDef.density = m_Density;
		fixtureDef.filter = GetFilter();
		fixtureDef.friction = m_Friction;
		fixtureDef.restitution = m_Restitution;

		b2PolygonShape box;
		box.SetAsBox(m_Width * scale.x / 2.f / M_PPM, m_Height * scale.y / 2.f / M_PPM, { m_Offset.x / M_PPM, m_Offset.y / M_PPM }, 0);
		fixtureDef.shape = &box;
		fixtureDef.isSensor = m_IsSensor;
		fixtureDef.userData = this;
		
		if (m_pFicture)
			m_pRigidbody->DestroyShape(m_pFicture);
		m_pFicture = m_pRigidbody->AddShape(fixtureDef);
	}
}

void BoxColliderComponent::RegisterCollision(GameObject* pObject, GameObject* collWith, bool begin)
{
	if (!pObject || !m_pRigidbody)
		return;

	const std::string text = begin ? "Entered" : "Exited";

	for (auto tag : collWith->GetTags())
	{
		std::cout << tag + text << "\n";
		m_pRigidbody->GetSubject()->Notify(tag + text, pObject, collWith);
	}
}


void BoxColliderComponent::LoadSettings(const std::string& settings)
{
	if (settings == "Player")
		LoadPlayerSettings(false);
	else if (settings == "PlayerOverlap")
		LoadPlayerSettings(true);
	else if (settings == "Bubble")
		LoadBubbleSettings(false);
	else if (settings == "BubbleOverlap")
		LoadBubbleSettings(true);
	else if (settings == "ZenChan")
		LoadZenChanSettings();
}

void BoxColliderComponent::LoadPlayerSettings(bool overlap)
{
	m_CollisionGroup = CollisionGroup::Five;
	for (int i{}; i < m_NrOfCollGroups; i++)
		m_IgnoreGroups[i] = false;
	m_IgnoreGroups[4] = true;
	m_SelectedCollGroupIndex = 4;

	m_Density = 10.f;
	m_Friction = 0.65f;
	m_Restitution = 0.3f;
	m_IsSensor = overlap;
	if (m_pFicture)
	{
		SetCollisionGroups();
		m_pFicture->SetDensity(m_Density);
		m_pFicture->SetFriction(m_Friction);
		m_pFicture->SetRestitution(m_Restitution);
		m_pFicture->SetSensor(m_IsSensor);
	}

	m_Width = 16.f;
	m_Height = 16.f;
	m_RenderCollider = false;
	CreateShape();
}

void BoxColliderComponent::LoadBubbleSettings(bool overlap)
{
	m_CollisionGroup = CollisionGroup::Four;
	for (int i{}; i < m_NrOfCollGroups; i++)
		m_IgnoreGroups[i] = false;

	m_SelectedCollGroupIndex = 3;
	m_IsSensor = overlap;

	if (m_pFicture)
	{
		m_pFicture->SetSensor(m_IsSensor);
		SetCollisionGroups();
	}

	m_Width = 16.f;
	m_Height = 16.f;
	m_RenderCollider = false;
	CreateShape();
}

void BoxColliderComponent::LoadZenChanSettings()
{
	m_CollisionGroup = CollisionGroup::Three;
	for (int i{}; i < m_NrOfCollGroups; i++)
		m_IgnoreGroups[i] = false;
	m_IgnoreGroups[3] = true; // No Player collision
	m_IgnoreGroups[2] = true; // No self collision
	m_SelectedCollGroupIndex = 2;

	m_Density = 10.f;
	m_Friction = 0.65f;
	m_Restitution = 0.3f;
	if (m_pFicture)
	{
		SetCollisionGroups();
		m_pFicture->SetDensity(m_Density);
		m_pFicture->SetFriction(m_Friction);
		m_pFicture->SetRestitution(m_Restitution);
	}
}
