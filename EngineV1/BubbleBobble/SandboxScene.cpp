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
	auto gameObject = std::make_shared<GameObject>();
	auto textureComp = new TextureComponent(std::weak_ptr<GameObject>(gameObject), "background.jpg");
	gameObject->AddComponent(textureComp);
	Add(gameObject);

	//--------Logo--------
	gameObject = std::make_shared<GameObject>();
	textureComp = new TextureComponent(std::weak_ptr<GameObject>(gameObject), "logo.png");
	textureComp->SetPosition(glm::vec3(216, 180, 0));
	gameObject->AddComponent(textureComp);
	Add(gameObject);

	//--------Text--------
	gameObject = std::make_shared<GameObject>();
	auto font = ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36);
	auto textComp = new TextComponent(std::weak_ptr<GameObject>(gameObject), font, "Programming 4 Assignment");
	textComp->SetPosition(glm::vec3(80, 20, 0));
	gameObject->AddComponent(textComp);
	Add(gameObject);

	//--------FPS Counter--------
	gameObject = std::make_shared<GameObject>();
	m_FpsCounter = std::weak_ptr<GameObject>(gameObject);
	textComp = new TextComponent(std::weak_ptr<GameObject>(gameObject), font, "sadsad");
	textComp->SetPosition(glm::vec3(80, 80, 0));
	gameObject->AddComponent(textComp);
	Add(gameObject);

}

void SandboxScene::Update(float elapsedSec)
{
	//--------Update FPS Counter--------
	m_FpsCounter.lock()->GetComponent<TextComponent>()->SetText(std::to_string(1 / elapsedSec));
}
