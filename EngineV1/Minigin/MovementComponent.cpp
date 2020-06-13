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
			if (InputManager::GetInstance()->IsActionDown("MoveLeft"))
				pRigidbody->MoveHorizontal({ -m_Speed , 0 });// Not * elapsedSec since there is no acceleration
			else if (InputManager::GetInstance()->IsActionDown("MoveRight"))
				pRigidbody->MoveHorizontal({ m_Speed , 0 });// Not * elapsedSec since there is no acceleration
			if (InputManager::GetInstance()->IsActionPressed("Jump"))
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

void MovementComponent::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Speed", FloatToXMLChar(doc, m_Speed)));
	node->append_attribute(doc->allocate_attribute("JumpStrength", FloatToXMLChar(doc, m_JumpStrength)));
}

void MovementComponent::SetAttributes(float speed, float jumpStrength)
{
	m_Speed = speed;
	m_JumpStrength = jumpStrength;
}

void MovementComponent::LoadSettings(const std::string settings)
{
	if (settings == "Player")
		LoadPlayerSettings();
}

void MovementComponent::LoadPlayerSettings()
{
	m_JumpStrength = 27.f;
	m_Speed = 1.f;
}
