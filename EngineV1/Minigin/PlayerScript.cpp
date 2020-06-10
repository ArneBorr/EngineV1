#include "MiniginPCH.h"
#include "PlayerScript.h"
#include "imgui.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "AnimatorControllerComponent.h"

PlayerScript::PlayerScript()
	: Script("PlayerScript")
{

}


void PlayerScript::Initialize()
{
	m_pAnimator = m_pGameObject->GetComponent<AnimatorControllerComponent>();
	m_pRigidbody = m_pGameObject->GetRigidbody();
}

void PlayerScript::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
	if (m_pGameObject)
	{
		if (m_pAnimator && m_pRigidbody)
		{
			const auto vel = m_pRigidbody->GetVelocity();
			if (abs(vel.x) > FLT_EPSILON)
				m_pAnimator->Play(true);
			else
				m_pAnimator->Play(false);
		}
	}
}

void PlayerScript::DrawInterface()
{
	
}
