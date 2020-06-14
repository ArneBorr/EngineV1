#include "MiniginPCH.h"
#include "MainMenu.h"
#include "InputManager.h"
#include "SceneManager.h"

MainMenu::MainMenu()
	: Script{ "MainMenu" }
{
}

void MainMenu::Update(float)
{
	if (InputManager::GetInstance()->IsActionPressed("SoloSelect", PlayerAction::All))
		SceneManager::GetInstance()->SetScene("SoloLevel_1");
	else if (InputManager::GetInstance()->IsActionPressed("CoopSelect", PlayerAction::All))
		SceneManager::GetInstance()->SetScene("CoopLevel_1");
}
