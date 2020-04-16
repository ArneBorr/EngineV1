#include "stdafx.h"
#include "SandboxScene.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "TextureComponent.h"

SandboxScene::SandboxScene()
	: Scene()
{
}

void SandboxScene::Initialize()
{
	//Background
	auto gameObject = new GameObject();
	auto textureComp = new TextureComponent(gameObject, "background.jpg");
	gameObject->AddComponent(textureComp);
	Add(gameObject);

	//--------Logo--------
	gameObject = new GameObject();
	textureComp = new TextureComponent(gameObject, "logo.png");
	textureComp->SetPosition(glm::vec3(216, 180, 0));
	gameObject->AddComponent(textureComp);
	Add(gameObject);

	//--------Text--------
	gameObject = new GameObject();
	auto font = ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36);
	auto textComp = new TextComponent(gameObject, font, "Programming 4 Assignment");
	textComp->SetPosition(glm::vec3(80, 20, 0));
	gameObject->AddComponent(textComp);
	Add(gameObject);

	//--------FPS Counter--------
	gameObject = new GameObject();
	m_pFpsCounter = gameObject;
	textComp = new TextComponent(gameObject, font, "sadsad");
	textComp->SetPosition(glm::vec3(80, 80, 0));
	gameObject->AddComponent(textComp);
	Add(gameObject);

}

void SandboxScene::Update(float elapsedSec)
{
	//--------Update FPS Counter--------
	m_pFpsCounter->GetComponent<TextComponent>()->SetText(std::to_string(1 / elapsedSec));
}
