#include "MiniginPCH.h"
#include "Behaviour.h"
#include "imgui.h"
#include "FSMComponent.h"
#include "IdleBehaviour.h"
#include "Sprite.h"

Behaviour::Behaviour(const std::string& name)
	: m_Name{ name }
{
}

Behaviour::~Behaviour()
{
	// No deletion since object does not own the objects he "has"(transitions and sprite are all stored by the FSM)
}

void Behaviour::Enter()
{
	if (m_pSprite)
	{
		m_pSprite->Reset();
		m_pSprite->Play(true);
	}
}

void Behaviour::Update(float elapsesSec)
{
	if (m_pSprite)
		m_pSprite->Update(elapsesSec);
}

void Behaviour::Render()
{
	if (m_pSprite)
		m_pSprite->Render();
}

void Behaviour::GetTransitionsAndSpriteFromAtrribute(std::vector<std::string>& transitions, rapidxml::xml_node<>* node, std::string& sprite)
{
	node->remove_first_attribute(); // Remove name
	const int nrOfTransitions = std::stoi(node->first_attribute("NrOfTransitions")->value());
	node->remove_first_attribute();

	for (int i{}; i < nrOfTransitions; i++)
	{
		auto attribute = node->first_attribute("Transition");
		if (attribute != 0)
		{
			std::string transition = attribute->value();
			transitions.push_back(transition);
			node->remove_first_attribute();
		}
		else
		{
			break;
		}
	}

	auto temp = node->first_attribute("Sprite");
	if (temp != 0)
		sprite = temp->value();
}

Behaviour* Behaviour::HandleTransitionDrop(Behaviour*)
{
	using namespace ImGui;
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Behaviour"))
		{
			auto temp = m_pFSM->GetDraggedBehaviour();
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
