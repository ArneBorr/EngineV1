#include "MiniginPCH.h"
#include "TransformComponent.h"
#include "imgui.h"
#include "SceneManager.h"
#include "RigidbodyComponent.h"

TransformComponent::TransformComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject, "TransformComponent")
{
	m_Position = SceneManager::GetInstance()->AdapatPositionToEditor(Vector2f{ 0, 0 });
	UpdateTransform(false);
}

void TransformComponent::Initialize()
{
	m_InitialPosition = m_Position;
	m_InitialRotation = m_Rotation;
}

void TransformComponent::DrawInterface()
{
	using namespace ImGui;
	SetNextItemOpen(true, ImGuiCond_Once);
	PushID(this);
	bool open = TreeNode(&GetName().front());
	HandleDrag();

	if (open)
	{
		Separator();
		Spacing();

		Text("Position");

		//PushItemWidth(50.f);
		if (InputFloat("X", &m_Position.x, 1.f, 50.f, "%.1f"))
			m_pGameObject->SetTransformChanged(true);
		if (InputFloat("Y", &m_Position.y, 1.f, 50.f, "%.1f"))
			m_pGameObject->SetTransformChanged(true);

		Text("Rotation");

		PushItemWidth(50.f);
		if (InputFloat("Angle", &m_Rotation))
			m_pGameObject->SetTransformChanged(true);
		Clamp(m_Rotation, 0.f, 360.f);

		Text("Scale");
		if (InputFloat("Width", &m_Scale.x))
			m_pGameObject->SetTransformChanged(true);
		SameLine();
		if (InputFloat("Height", &m_Scale.y))
			m_pGameObject->SetTransformChanged(true);

		TreePop();
	}

	HandleDrop();
	PopID();
}

void TransformComponent::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("PosX", FloatToXMLChar(doc, m_Position.x)));
	node->append_attribute(doc->allocate_attribute("PosY", FloatToXMLChar(doc, m_Position.y)));
	node->append_attribute(doc->allocate_attribute("ScaleX", FloatToXMLChar(doc, m_Scale.x)));
	node->append_attribute(doc->allocate_attribute("ScaleY", FloatToXMLChar(doc, m_Scale.y)));
	node->append_attribute(doc->allocate_attribute("Rot", FloatToXMLChar(doc, m_Rotation)));
}

void TransformComponent::SetAttributes(const Vector2f& pos, const Vector2f& scale, float rot)
{
	m_Position = pos;
	m_Scale = scale;
	m_Rotation = rot;
	UpdateTransform(true);
}

void TransformComponent::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
	m_pGameObject->SetTransformChanged(true);
}

void TransformComponent::SetPosition(const Vector2f& pos)
{
	SetPosition(pos.x, pos.y);
}

void TransformComponent::SetRotation(float rot)
{
	m_Rotation = rot;
	m_pGameObject->SetTransformChanged(true);
}

void TransformComponent::SetScale(float x, float y)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_pGameObject->SetTransformChanged(true);
}

void TransformComponent::SetScale(const Vector2f& scale)
{
	SetScale(scale.x, scale.y);
}

void TransformComponent::UpdateTransform(bool updateBody)
{
	m_WorldRotation = m_Rotation;
	m_WorldScale = m_Scale;
	m_WorldPosition = m_Position;

	//Adapt to parent
	auto parent = m_pGameObject->GetParent();
	if (parent)
	{
		auto parentTrans = parent->GetTransform();
		if (parentTrans)
		{
			m_WorldPosition.x += parentTrans->GetWorldPosition().x;
			m_WorldPosition.y += parentTrans->GetWorldPosition().y;
			m_WorldRotation += parentTrans->GetWorldRotation();
			m_WorldScale.x *= parentTrans->GetWorldScale().x;
			m_WorldScale.y *= parentTrans->GetWorldScale().y;
		}
	}

	if (updateBody)
	{
		auto rigidbody = m_pGameObject->GetRigidbody();
		if (rigidbody)
		{
			rigidbody->SetPosition({ m_WorldPosition.x / M_PPM,  m_WorldPosition.y / M_PPM });
			rigidbody->SetRotation(m_WorldRotation);
			rigidbody->UpdateShapeScale();
		}
	}
	

	m_pGameObject->SetTransformChanged(false);
}

void TransformComponent::Reset()
{
	m_Position = m_InitialPosition;
	m_Rotation = m_InitialRotation;
	m_pGameObject->SetTransformChanged(true);
}

void TransformComponent::LoadSettings(const std::string& settings)
{
	if (settings == "Player")
		LoadPlayerSettings();
}

void TransformComponent::LoadPlayerSettings()
{
	m_Scale = { 2.f, 2.f };
	UpdateTransform(false);
}

