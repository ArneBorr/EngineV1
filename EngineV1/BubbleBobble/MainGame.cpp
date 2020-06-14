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
#include "GameObjectManager.h"
#include "SaveHandler.h"
#include <SDL_mixer.h>
#include "SoundManager.h"

using namespace std;

const float MainGame::m_MsPerUpdate = 1 / 60.f;

MainGame::MainGame()
{
}

void MainGame::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	unsigned int xWindow = 1280, yWindow = 750;

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

	
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		std::cout << "Error:" << Mix_GetError() << std::endl;

	GameInfo::GetInstance()->SetWindowSize({ float(xWindow) , float(yWindow) });
	Renderer::GetInstance()->Initialize(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void MainGame::LoadGame() const
{
	GameObjectManager::GetInstance()->Initialize();
	SaveHandler* pSaveHandler = new SaveHandler();
	InputManager::GetInstance()->Initialize(pSaveHandler);
	SceneManager::GetInstance()->Initialize(pSaveHandler);
	SoundManager::GetInstance()->Initialize(pSaveHandler);
	GameInfo::GetInstance()->Initialize(pSaveHandler);
}

void MainGame::Cleanup()
{
	GameInfo::GetInstance()->Destroy();
	Renderer::GetInstance()->Destroy();
	SceneManager::GetInstance()->Destroy();
	InputManager::GetInstance()->Destroy();
	GameObjectManager::GetInstance()->Destroy();
	ResourceManager::GetInstance()->Destroy();
	SoundManager::GetInstance()->Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	Mix_Quit();
	SDL_Quit();
}

void MainGame::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance()->Init("Data/");

	LoadGame();

	{
		auto gameInfo = GameInfo::GetInstance();
		auto renderer = Renderer::GetInstance();
		auto sceneManager = SceneManager::GetInstance();
		auto input = InputManager::GetInstance();

		gameInfo->StartTimer();
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
				sceneManager->Update(m_MsPerUpdate);
				lag -= m_MsPerUpdate;
			}
			
			renderer->Render();
		}
	}

	Cleanup();
}
