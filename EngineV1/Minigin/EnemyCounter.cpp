#include "MiniginPCH.h"
#include "EnemyCounter.h"
#include "imgui.h"
#include "SceneManager.h"

EnemyCounter::EnemyCounter()
	: Script("EnemyCounter")
{
}

void EnemyCounter::DrawInterface()
{
	ImGui::InputFloat("AmountOfEnemies", &m_AmountOfEnemies);
	ImGui::InputFloat("TimeBeforeRestart", &m_TimeBeforeRestart);
}

void EnemyCounter::Update(float elapsedSec)
{
	if (m_AmountOfEnemies <= 0)
	{
		m_Timer += elapsedSec;
		if (m_Timer >= m_TimeBeforeRestart)
		{
			SceneManager::GetInstance()->SetForReset();
			SceneManager::GetInstance()->SetScene("MainMenu");
		}
	}
}

void EnemyCounter::OnNotify(const std::string& event, GameObject*, GameObject*)
{
	if (event == "EnemyDead")
		--m_AmountOfEnemies;
}

void EnemyCounter::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	node->append_attribute(doc->allocate_attribute("AmountOfEnemies", FloatToXMLChar(doc, m_AmountOfEnemies)));
	node->append_attribute(doc->allocate_attribute("TimeBeforeRestart", FloatToXMLChar(doc, m_TimeBeforeRestart)));
}

void EnemyCounter::SetAttributes(rapidxml::xml_node<>* node)
{
	m_AmountOfEnemies = std::stof(node->first_attribute("AmountOfEnemies")->value());
	m_TimeBeforeRestart = std::stof(node->first_attribute("TimeBeforeRestart")->value());
}
