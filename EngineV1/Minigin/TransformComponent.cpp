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
		

		Text("Scale");
		InputFloat("Width", &m_Scale.x);
		SameLine();
		InputFloat("Height", &m_Scale.y);;

		TreePop();
	}

	HandleDrop();
}

void TransformComponent::SetPosition(float x, float y)
{
	auto pos = SceneManager::GetInstance()->AdaptLocationToEditor(Vector2f{ x, y });
	m_Position.x = x;
	m_Position.y = y;
}

void TransformComponent::SetPosition(Vector2f pos)
{
	SetPosition(pos.x, pos.y);
}

void TransformComponent::SetScale(float x, float y)
{
	m_Scale.x = x;
	m_Scale.y = y;
}

void TransformComponent::SetScale(Vector2f scale)
{
	SetScale(scale.x, scale.y);
}
