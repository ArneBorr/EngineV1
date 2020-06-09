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
		auto animator = m_pGameObject->GetComponent<AnimatorControllerComponent>();
		auto rigidbody = m_pGameObject->GetRigidbody();
		if (animator && rigidbody)
		{
			const auto vel = rigidbody->GetVelocity();
			if (abs(vel.x) > FLT_EPSILON)
				animator->Play(true);
			else
				animator->Play(false);
		}
	}
}

void PlayerScript::DrawInterface()
{
	ImGui::Button("OK");
	ImGui::Button("OK");
	ImGui::Button("OK");
	ImGui::Button("OK");
	ImGui::Button("OK");
}
