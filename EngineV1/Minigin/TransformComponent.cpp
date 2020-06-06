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
	m_pGameObject->TransformChanged(true);
}

void TransformComponent::SetPosition(const Vector2f& pos)
{
	SetPosition(pos.x, pos.y);
}

void TransformComponent::SetRotation(float rot)
{
	m_Rotation = rot;
}

void TransformComponent::SetScale(float x, float y)
{
	m_Scale.x = x;
	m_Scale.y = y;
}

void TransformComponent::SetScale(const Vector2f& scale)
{
	SetScale(scale.x, scale.y);
	m_pGameObject->TransformChanged(true);
}
