#include "MiniginPCH.h"
#include "Behaviour.h"
#include "imgui.h"
#include "FSMComponent.h"
#include "IdleBehaviour.h"

Behaviour::Behaviour(const std::string& name)
	: m_Name{ name }
{
}

Behaviour::~Behaviour()
{
	// No deletion since object does not own the objects he "has", he just holds a reference
}

Behaviour* Behaviour::HandleTransitionDrop(Behaviour* pThis)
{
	using namespace ImGui;
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Behaviour"))
		{
			auto temp = m_pFSM->GetDraggedBehaviour();
			if (temp != pThis)
				return temp;
		}

		ImGui::EndDragDropTarget();
	}

	return nullptr;
}

Sprite* Behaviour::HandleSpriteDrop()
{
	using namespace ImGui;
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Sprite"))
		{
			return m_pFSM->GetDraggedSprite();
		}

		ImGui::EndDragDropTarget();
	}

	return nullptr;
}

void Behaviour::PrintTransitionSet(Behaviour* pBehaviour)
{
	using namespace ImGui;
	if (pBehaviour)
	{
		SameLine();
		Text(pBehaviour->GetName().c_str());
	}
}

void Behaviour::TransitionButtonInteractions(Behaviour* pBehaviour)
{
	auto temp = HandleTransitionDrop(this);
	if (temp)
		pBehaviour = temp;

	if (pBehaviour)
	{
		ImGui::SameLine();
		ImGui::Text(pBehaviour->GetName().c_str());
	}
}
