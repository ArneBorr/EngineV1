#include "MiniginPCH.h"
#include "MovementComponent.h"
#include "RigidbodyComponent.h"
#include "imgui.h"
#include "InputManager.h"

MovementComponent::MovementComponent(GameObject* pObject)
	: BaseComponent(pObject, "MovementComponent")
{
}

void MovementComponent::Render()
{
}

void MovementComponent::Update(float)
{
	if (GameInfo::GetInstance()->IsPlaying())
	{
		auto pRigidbody = m_pGameObject->GetRigidbody();
		if (pRigidbody)
		{
			if (InputManager::GetInstance()->IsActionDown("MoveLeft", PlayerAction::All))
				pRigidbody->MoveHorizontal({ -m_Speed , 0 });// Not * elapsedSec since there is no acceleration
			else if (InputManager::GetInstance()->IsActionDown("MoveRight", PlayerAction::All))
				pRigidbody->MoveHorizontal({ m_Speed , 0 });// Not * elapsedSec since there is no acceleration
			if (InputManager::GetInstance()->IsActionPressed("Jump", PlayerAction::All))
				pRigidbody->Jump(m_JumpStrength);
		}
	}	
}

void MovementComponent::DrawInterface()
{
	using namespace ImGui;

	SetNextItemOpen(true, ImGuiCond_Once);
	bool open = TreeNode(&GetName().front());
	HandleDrag();

	if (open)
	{
		Separator();
		Spacing();
		InputFloat("Speed", &m_Speed);
		InputFloat("JumpStrength", &m_JumpStrength);
		TreePop();
	}

	HandleDrop();
}

void MovementComponent::SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode)
{
	pNode->append_attribute(pDoc->allocate_attribute("Speed", FloatToXMLChar(pDoc, m_Speed)));
	pNode->append_attribute(pDoc->allocate_attribute("JumpStrength", FloatToXMLChar(pDoc, m_JumpStrength)));
}

void MovementComponent::SetAttributes(float speed, float jumpStrength)
{
	m_Speed = speed;
	m_JumpStrength = jumpStrength;
}
