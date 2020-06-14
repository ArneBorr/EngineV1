#pragma once
#include "Script.h"

class TextComponent;

class HUD : public Script
{
public:
	HUD();

	void Initialize() override;
	void LateInitialize() override;
	void DrawInterface() override;
	void OnNotify(const std::string& event, GameObject* pObject, GameObject* collWithObj) override;

private:

	TextComponent* m_pScoreP1Text{ nullptr };
	TextComponent* m_pLivesP1Text{ nullptr };
	TextComponent* m_pScoreP2Text{ nullptr };
	TextComponent* m_pLivesP2Text{ nullptr };
	int m_ScoreP1{};
	int m_LivesP1{};
	int m_ScoreP2{};
	int m_LivesP2{};
	int m_StartingLives{ 4 };
};

