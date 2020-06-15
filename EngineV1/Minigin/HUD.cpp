#include "MiniginPCH.h"
#include "HUD.h"
#include "imgui.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "PickUp.h"
#include "ScriptComponent.h"
#include "SceneManager.h"

HUD::HUD()
	: Script("HUD")
{
}

void HUD::Initialize()
{
	m_LivesP1 = m_StartingLives;
	m_LivesP2 = m_StartingLives;
}

void HUD::LateInitialize()
{
	auto textComponents = m_pGameObject->GetComponents<TextComponent>();
	for (auto textComp : textComponents)
	{
		if (textComp->GetText() == "L1:")
		{
			m_pLivesP1Text = textComp;
			m_pLivesP1Text->SetText("L1: " + std::to_string(m_LivesP1));
		}
		else if (textComp->GetText() == "L2:")
		{
			m_pLivesP2Text = textComp;
			m_pLivesP2Text->SetText("L2: " + std::to_string(m_LivesP2));
		}
		else if (textComp->GetText() == "S1:")
		{
			m_pScoreP1Text = textComp;
			m_pScoreP1Text->SetText("S1: " + std::to_string(m_ScoreP1));
		}
		else if (textComp->GetText() == "S2:")
		{
			m_pScoreP2Text = textComp;
			m_pScoreP2Text->SetText("S2: " + std::to_string(m_ScoreP2));
		}
	}
}

void HUD::DrawInterface()
{
	ImGui::InputInt("StartingLives", &m_StartingLives);
}

void HUD::OnNotify(const std::string& event, GameObject* pObj, GameObject* pObjCollWith)
{
	if (event == "PickedUpItem")
	{
		auto scriptComponents = pObj->GetComponents<ScriptComponent>();
		PickUp* pickUp = nullptr;
		for (auto scriptComp : scriptComponents)
		{
			auto script = scriptComp->GetScript();
			if (script)
			{
				pickUp = dynamic_cast<PickUp*>(script);
				if (pickUp)
					break;
			}
		}

		if (pickUp)
		{
			if (pObjCollWith->HasTags({ "Player1" }))
			{
				m_ScoreP1 += int(pickUp->GetPoints());
				m_pScoreP1Text->SetText("S1: " + std::to_string(m_ScoreP1));
			}
			else
			{
				m_ScoreP2 += int(pickUp->GetPoints());
				m_pScoreP1Text->SetText("S2: " + std::to_string(m_ScoreP2));
			}
		}
	}
	else if (event == "Player1Hit")
	{
		--m_LivesP1;
		m_pLivesP1Text->SetText("L1: " + std::to_string(m_LivesP1));

		if (m_LivesP1 == 0)
			SceneManager::GetInstance()->SetForReset();
	}
	else if (event == "Player2Hit")
	{
		--m_LivesP2;
		m_pLivesP2Text->SetText("L2: " + std::to_string(m_LivesP2));
		if (m_LivesP2 == 0)
			SceneManager::GetInstance()->SetForReset();
	}
}