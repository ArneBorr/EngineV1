#include "MiniginPCH.h"
#include "AllowOneWay.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"

AllowOneWay::AllowOneWay()
	: Script("AllowOneWay")
{
}

void AllowOneWay::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
}

void AllowOneWay::Update(float)
{
	if (m_pRigidbody)
	{
		if (!m_IsCollisionIgnored && (m_pRigidbody->GetVelocity().y < 0)) //negative is up
		{
			m_pRigidbody->SetIgnoreGroup(2, true); // Group 2 are platforms
			m_IsCollisionIgnored = true;
		}
		else if (m_IsCollisionIgnored && (m_pRigidbody->GetVelocity().y > 0))
		{
			m_pRigidbody->SetIgnoreGroup(2, false); // Group 2 are platforms
			m_IsCollisionIgnored = false;
		}
	}
}

void AllowOneWay::DrawInterface()
{
}
