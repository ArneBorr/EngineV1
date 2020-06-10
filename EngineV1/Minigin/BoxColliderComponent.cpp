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
		box.SetAsBox(m_Width * scale.x / 2.f / M_PPM, m_Height * scale.y / 2.f / M_PPM);
		pBody->ChangeShape(this, box);
	}

	m_pTexture = ResourceManager::GetInstance()->LoadTexture("BoxOutline.png");
}

BoxColliderComponent::~BoxColliderComponent()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

BoxColliderComponent::BoxColliderComponent(const BoxColliderComponent& other) noexcept
	: BaseComponent(other.m_pGameObject, other.m_Name + " - Copy")
{
	m_pRigidbody = other.m_pRigidbody;
	m_pTexture = new Texture2D{ *other.m_pTexture };
	m_Width = other.m_Width;
	m_Height = other.m_Height;
	m_RenderCollider = other.m_RenderCollider;
}

void BoxColliderComponent::Render()
{
	if (!m_RenderCollider)
		return;

	auto transform = m_pGameObject->GetTransform();
	if (!transform)
		return;

	const Vector2f pos = transform->GetWorldPosition();
	const Vector2f scale = transform->GetWorldScale();
	const float scaleX = m_Width / 300.f * scale.x; // Texture width = 300;
	const float scaleY = m_Height / 300.f * scale.y; // Texture width = 300;
	if (m_pRigidbody)
		Renderer::GetInstance()->RenderTexture(*m_pTexture, pos, { scaleX, scaleY }, m_pRigidbody->GetRotation(), true);
	else
		Renderer::GetInstance()->RenderTexture(*m_pTexture, pos, { scaleX, scaleY }, transform->GetWorldRotation(), true);
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

		PushItemWidth(150.f);
		if (InputFloat("Width", &m_Width, 1.f, 50.f, "%.1f"))
			CreateShape();
		PushItemWidth(150.f);
		if (InputFloat("Height", &m_Height, 1.f, 50.f, "%.1f"))
			CreateShape();

		Checkbox("Render Collider", &m_RenderCollider);
		TreePop();
	}

	HandleDrop();
}

void BoxColliderComponent::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Width", FloatToXMLChar(doc, m_Width)));
	node->append_attribute(doc->allocate_attribute("Height", FloatToXMLChar(doc, m_Height)));
	node->append_attribute(doc->allocate_attribute("RenderCollider", IntToXMLChar(doc, m_RenderCollider)));
	CreateShape();
}

void BoxColliderComponent::SetAttributes(float width, float height, int renderCollider)
{
	m_Width = width;
	m_Height = height;
	m_RenderCollider = renderCollider;
	CreateShape();
}

void BoxColliderComponent::CreateLink(RigidbodyComponent* pBody)
{
	m_pRigidbody = pBody;
	CreateShape();
}

void BoxColliderComponent::SetObject(GameObject* pObject)
{
	auto pBody = pObject->GetRigidbody();
	if (pBody)
	{
		auto transform = pObject->GetTransform();
		Vector2f scale{ 1, 1 };
		if (transform)
			scale = transform->GetWorldScale();

		b2PolygonShape box;
		box.SetAsBox(m_Width * scale.x / 2.f / M_PPM, m_Height * scale.y / 2.f / M_PPM);
		pBody->ChangeShape(this, box);
	}

	BaseComponent::SetObject(pObject);
}

void BoxColliderComponent::CreateShape()
{
	Vector2f scale = m_pGameObject->GetTransform()->GetWorldScale();
	
	if (m_pRigidbody && m_Width != 0 && m_Height != 0 )
	{		
		if (abs(scale.x) - 0 < FLT_EPSILON)
			scale.x = 0.0001f;
		if (abs(scale.y) - 0 < FLT_EPSILON)
			scale.y = 0.0001f;

		b2PolygonShape box;
		box.SetAsBox(m_Width  * scale.x / 2.f / M_PPM, m_Height  * scale.y / 2.f / M_PPM);
		m_pRigidbody->ChangeShape(this, box);
	}
}
