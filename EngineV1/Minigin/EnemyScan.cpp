#include "MiniginPCH.h"
#include "EnemyScan.h"
#include "imgui.h"
#include "Sprite.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "BoxColliderComponent.h"

EnemyScan::EnemyScan()
	:Behaviour("EnemyScan")
{
}

void EnemyScan::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf(" EnemyScan::Initialize() : No Rigidbody Found!");
}

void EnemyScan::Enter()
{
	if (m_pRigidbody)
		m_pRigidbody->SetVelocity({ 0, 0 });

	std::cout << "Scan\n";

	m_IsLeftChecked = false;
	m_IsRightChecked = false;
	m_Timer = 0;
	if (m_pSprite)
		m_pSprite->Play(false);
}

Behaviour* EnemyScan::HandleInput()
{
	//Raycast
	Vector2f pos{};
	if (m_pRigidbody)
	{
		pos = m_pRigidbody->GetPosition();
	}

	int sign = m_IsLeftChecked ? 1 : -1;
	auto closestFicture = m_pGameObject->GetScene()->RayCast({ pos.x / M_PPM, pos.y / M_PPM }, { pos.x / M_PPM + m_ViewRangePlayer * sign / M_PPM, pos.y / M_PPM });

	GameObject* closesttObject{ nullptr };
	if (closestFicture)
	{
		closesttObject = static_cast<BoxColliderComponent*>(closestFicture->GetUserData())->GetGameObject();
		//Is looking at player
		if (closesttObject->HasTags({ "Player" }))
			return m_pRunTransition;
	}

	//Has scannes everything
	if (m_IsLeftChecked && m_IsRightChecked)
		return m_pRunTransition;

	return nullptr;
}

void EnemyScan::Update(float elapsedSec)
{
	m_Timer += elapsedSec;
	
	if (!m_IsLeftChecked)
	{
		if (m_pRigidbody)
			m_pRigidbody->MoveHorizontal({ -0.1f, 0 });// Not * elapsedSec since there is no acceleration

		m_pSprite->Flip(true);

		if (m_Timer > m_MaxCheckTime)
		{
			m_pRigidbody->SetVelocity({ 0, 0 });
			m_IsLeftChecked = true;
			m_Timer = 0;
			
			std::cout << "LEft Checked\n";
		}
	}
	else if (!m_IsRightChecked)
	{
		if (m_pRigidbody)
			m_pRigidbody->MoveHorizontal({ 0.1f, 0 });// Not * elapsedSec since there is no acceleration

		m_pSprite->Flip(false);

		if (m_Timer > m_MaxCheckTime)
		{
			m_pRigidbody->SetVelocity({ 0, 0 });
			m_IsRightChecked = true;
			m_Timer = 0;	
		}
	}
}

void EnemyScan::DrawInterface()
{
	using namespace ImGui;
	Separator();
	Text("Transitions");
	Separator();

	//Set transition, A lot of repetition because of the way imgui works
	Behaviour* temp;
	if (Button("RunTransition"))
		m_pRunTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pRunTransition = temp;
	PrintTransitionSet(m_pRunTransition);

	//Sprite
	Separator();
	Text("Sprite");
	Separator();
	if (m_pSprite)
		Button(m_pSprite->GetName().c_str());
	else
		Button("None");
	auto sprite = HandleSpriteDrop();
	if (sprite)
		m_pSprite = sprite;
	Separator();
}

void EnemyScan::SaveAttributes(rapidxml::xml_document<>*, rapidxml::xml_node<>*)
{
}

void EnemyScan::SetAttributes(rapidxml::xml_node<>*)
{
}

void EnemyScan::SetTransitionsAndSprite(const std::vector<Behaviour*>& pTransitions, Sprite* pSprite)
{
	if (pTransitions.size() == 1)
	{
		m_pRunTransition = pTransitions[0];
	}

	m_pSprite = pSprite;
}
