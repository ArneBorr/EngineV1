#include "stdafx.h"
#include "MainGame.h"
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "SandboxScene.h"

using namespace std;

const float MainGame::m_MsPerUpdate = 0.016f; //0.016 for 60 fps, 0.033 for 30 fps

MainGame::MainGame()
{
}

void MainGame::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	
	unsigned int xWindow = 1280, yWindow = 720;

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		xWindow,
		yWindow,
		SDL_WINDOW_OPENGL
	);

	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	GameInfo::GetInstance()->SetWindowSize({ float(xWindow), float(yWindow) });
	Renderer::GetInstance()->Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void MainGame::LoadGame() const
{
	SceneManager::GetInstance()->AddScene(new SandboxScene());
	SceneManager::GetInstance()->Initialize();
}

void MainGame::Cleanup()
{
	GameInfo::GetInstance()->Destroy();
	SceneManager::GetInstance()->Destroy();
	InputManager::GetInstance()->Destroy();
	Renderer::GetInstance()->Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void MainGame::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance()->Init("../Data/");

	LoadGame();

	{
		auto gameInfo = GameInfo::GetInstance();
		auto renderer = Renderer::GetInstance();
		auto sceneManager = SceneManager::GetInstance();
		auto input = InputManager::GetInstance();

		gameInfo->Start();
		float lag = 0.f;

		bool doContinue = true;
		while (doContinue)
		{
			gameInfo->Update();
			
			auto elapsedSec = gameInfo->GetElapsedSeconds();
			lag += elapsedSec;

			doContinue = input->ProcessInput();

			while (lag >= m_MsPerUpdate)
			{
				sceneManager->Update(elapsedSec);
				lag -= m_MsPerUpdate;
			}
			
			renderer->Render();
			
		}
	}

	Cleanup();
}
