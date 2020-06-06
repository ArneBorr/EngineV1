#include "MiniginPCH.h"
#include "TransformComponent.h"
#include "imgui.h"
#include "SceneManager.h"

TransformComponent::TransformComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject, "TransformComponent")
{
	m_Position = SceneManager::GetInstance()->AdaptLocationToEditor(Vector2f{ 0, 0 });
}

void TransformComponent::DrawInterface()
{
	using namespace ImGui;
	SetNextItemOpen(true, ImGuiCond_Once);

	bool open = TreeNode(&GetName().front());
	HandleDrag();

	if (open)
	{
		Separator();
		Spacing();

		Text("Position");

		PushItemWidth(50.f);
		InputFloat("X", &m_Position.x);
		SameLine();
		InputFloat("Y", &m_Position.y);;

		Text("Rotation");

		PushItemWidth(50.f);
		InputFloat("Angle", &m_Rotation);
		Clamp(m_Rotation, 0.f, 360.f);

		Text("Scale");
		InputFloat("Width", &m_Scale.x);
		SameLine();
		InputFloat("Height", &m_Scale.y);;

		m_pGameObject->SetTransformChanged(true);
		TreePop();
	}

	HandleDrop();
}

void TransformComponent::SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc.allocate_attribute("PosX", FloatToXMLChar(doc, m_Position.x)));
	node->append_attribute(doc.allocate_attribute("PosY", FloatToXMLChar(doc, m_Position.y)));
	node->append_attribute(doc.allocate_attribute("ScaleX", FloatToXMLChar(doc, m_Scale.x)));
	node->append_attribute(doc.allocate_attribute("ScaleY", FloatToXMLChar(doc, m_Scale.y)));
}

void TransformComponent::SetPosition(float x, float y)
{
	auto pos = SceneManager::GetInstance()->AdaptLocationToEditor(Vector2f{ x, y });
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

void TransformComponent::UpdateTransform()
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

	m_pGameObject->SetTransformChanged(false);
}
