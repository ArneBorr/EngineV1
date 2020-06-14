#include "MiniginPCH.h"
#include "PickUp.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "imgui.h"
#include "Scene.h"
#include "Subject.h"
#include "SoundManager.h"

PickUp::PickUp()
	: Script("PickUp")
{
}

void PickUp::Initialize()
{
	
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (m_pRigidbody)
		m_pRigidbody->GetSubject()->AddObserver(this);
	else 
		std::printf("PickUp::Initialize() : No Rigidbody Found!");
}

void PickUp::DrawInterface()
{
	ImGui::InputFloat("Points", &m_Points);
}

void PickUp::OnNotify(const std::string& event, GameObject* pObject, GameObject*)
{
	if (event == "PlayerEntered" && pObject == m_pGameObject && m_pRigidbody)
	{
		SoundManager::GetInstance()->PlaySoundEffect("PickUp");
		m_pRigidbody->GetSubject()->RemoveObserver(this);
		m_pGameObject->GetScene()->DeleteChild(m_pGameObject);
	}
}


void PickUp::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	node->append_attribute(doc->allocate_attribute("Points", FloatToXMLChar(doc, m_Points)));
}

void PickUp::SetAttributes(rapidxml::xml_node<>* node)
{
	m_Points = std::stof(node->first_attribute("Points")->value());
}

