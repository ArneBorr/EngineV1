#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "imgui.h"

BaseComponent::BaseComponent(GameObject* pGameObject, const std::string& name)
	: m_pGameObject{ pGameObject }
	, m_Name{ name }
{
}

void BaseComponent::HandleDrag()
{
	// Drag this object to change the parent
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload("Component", &GetIndexHierarchy(), sizeof(*this), ImGuiCond_Once);
		ImGui::Text(&m_Name.front()); 

		ImGui::EndDragDropSource();
	}
}

void BaseComponent::HandleDrop()
{
	//Space to drop an item in between 2 other items
	ImGui::Selectable("          ", false, 0, { 499, 0.5f });
	//Drop another object on this to make the object a child
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* pPayload = ImGui::AcceptDragDropPayload("Component"))
		{
			auto index = *(unsigned int*)(pPayload->Data);

			m_pGameObject->ChangeComponentOrder(this, index);
		}

		ImGui::EndDragDropTarget();
	}
}
