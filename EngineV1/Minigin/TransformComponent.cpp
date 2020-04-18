#include "MiniginPCH.h"
#include "TransformComponent.h"
#include "imgui.h"

TransformComponent::TransformComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject, "TransformComponent")
{
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
		InputFloat("x", &m_Position.x);
		SameLine();
		InputFloat("y", &m_Position.y);
		SameLine();
		InputFloat("z", &m_Position.z);
		PopItemWidth();

		TreePop();
	}

	HandleDrop();
}

void TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}
