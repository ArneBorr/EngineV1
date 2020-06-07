#include "MiniginPCH.h"
#include "BoxColliderComponent.h"
#include "imgui.h"
#include "RigidbodyComponent.h"
#include "Renderer.h"
#include <SDL.h>
#include "TransformComponent.h"
#include "ResourceManager.h"
#include "Texture2D.h"

BoxColliderComponent::BoxColliderComponent(GameObject* pObject, RigidbodyComponent* pBody)
	: BaseComponent(pObject, "BoxColliderComponent"),
	m_pRigidbody{ pBody }
{
	if (pBody)
	{
		auto transform = pObject->GetTransform();
		Vector2f scale{ 1, 1 };
		if (transform)
			scale = transform->GetWorldScale();

		b2PolygonShape box;
		box.SetAsBox(m_Width / M_PPM * scale.x, m_Height / M_PPM * scale.y);
		pBody->ChangeShape(this, box);
	}

	m_pTexture = ResourceManager::GetInstance()->LoadTexture("BoxOutline.png");
}

BoxColliderComponent::~BoxColliderComponent()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void BoxColliderComponent::Render()
{
	auto transform = m_pGameObject->GetTransform();
	if (!transform)
		return;

	const Vector2f pos = transform->GetWorldPosition();
	const Vector2f scale = transform->GetWorldScale();
	const float scaleX = m_Width / 300.f * scale.x; // Texture width = 100;
	const float scaleY = m_Height / 300.f * scale.y; // Texture width = 100;
	Renderer::GetInstance()->RenderTexture(*m_pTexture, { pos.x - m_Width / 2.f, pos.y - m_Height / 2.f }, { scaleX, scaleY }, transform->GetRotation());
}

void BoxColliderComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
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

		if (ImGui::InputFloat("Width", &m_Width))
		{
			CreateShape();
		}

		if (ImGui::InputFloat("Height", &m_Height))
		{
			CreateShape();
		}

		TreePop();
	}

	HandleDrop();
}

void BoxColliderComponent::SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node)
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(node);

	node->append_attribute(doc.allocate_attribute("Width", FloatToXMLChar(doc, m_Width)));
	node->append_attribute(doc.allocate_attribute("Height", FloatToXMLChar(doc, m_Height)));
	CreateShape();
}

void BoxColliderComponent::SetAttributes(float width, float height)
{
	m_Width = width;
	m_Height = height;
	CreateShape();
}

void BoxColliderComponent::CreateLink(RigidbodyComponent* pBody)
{
	m_pRigidbody = pBody;
	CreateShape();
}

void BoxColliderComponent::CreateShape()
{
	if (m_pRigidbody)
	{
		const Vector2f scale = m_pGameObject->GetTransform()->GetScale();
		b2PolygonShape box;
		box.SetAsBox(m_Width / M_PPM * scale.x, m_Height / M_PPM * scale.y);
		m_pRigidbody->ChangeShape(this, box);
	}
}
