#include "MiniginPCH.h"
#include "Projectile.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "imgui.h"
#include "Scene.h"
#include "Subject.h"

Projectile::Projectile()
	: Script("Projectile")
{
}

void Projectile::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (m_pRigidbody)
		m_pRigidbody->GetSubject()->AddObserver(this);
	else
		std::printf("Projectile::Initialize() : Rigidbody Not Found!");
}

void Projectile::DrawInterface()
{
	ImGui::InputFloat("Speed", &m_Speed);
	ImGui::InputFloat("SurvivalTime", &m_SurvivalTime);
}

void Projectile::Update(float elapsedSec)
{
	m_Timer += elapsedSec;
	int sign = m_IsGoingLeft ? -1 : 1;
	m_pRigidbody->SetLinearVelocity({ m_Speed * sign, 0 });

	if (m_Timer > m_SurvivalTime)
	{
		m_pRigidbody->GetSubject()->RemoveObserver(this);
		m_pGameObject->GetScene()->DeleteChild(m_pGameObject);
	}
}

void Projectile::OnNotify(const std::string& event, GameObject* pObject, GameObject* pCollWithObj)
{
	if (event == "PlayerEntered" && pObject == m_pGameObject)
	{
		auto pRigidbody = pCollWithObj->GetRigidbody();
		if (pRigidbody)
			pRigidbody->GetSubject()->Notify("ProjectileEntered", pCollWithObj, pObject);
	}
}

void Projectile::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	node->append_attribute(doc->allocate_attribute("Speed", FloatToXMLChar(doc, m_Speed)));
	node->append_attribute(doc->allocate_attribute("SurvivalTime", FloatToXMLChar(doc, m_SurvivalTime)));
}

void Projectile::SetAttributes(rapidxml::xml_node<>* node)
{
	m_Speed = std::stof(node->first_attribute("Speed")->value());
	m_SurvivalTime = std::stof(node->first_attribute("SurvivalTime")->value());
}
