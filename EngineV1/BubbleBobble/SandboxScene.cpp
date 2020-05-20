#include "stdafx.h"
#include "SandboxScene.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "TextureComponent.h"

SandboxScene::SandboxScene()
	: Scene("Sandbox")
{
}

void SandboxScene::Initialize()
{
	

}

void SandboxScene::Update(float elapsedSec)
{
	Scene::Update(elapsedSec);
	UNREFERENCED_PARAMETER(elapsedSec);
	//--------Update FPS Counter--------
	//m_pFpsCounter->GetComponent<TextComponent>()->SetText(std::to_string(1 / elapsedSec));
}
