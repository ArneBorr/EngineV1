#include "MiniginPCH.h"
#include "BubblePopBehaviour.h"
#include "Sprite.h"
#include "imgui.h"
#include "SceneManager.h"
#include "Scene.h"
#include "FSMComponent.h"

BubblePopBehaviour::BubblePopBehaviour()
	: Behaviour("BubblePopBehaviour")
{
}

Behaviour* BubblePopBehaviour::HandleInput()
{
	return nullptr;
}

void BubblePopBehaviour::DrawInterface()
{
	using namespace ImGui;
	//Sprite
	Separator();
	Text("Sprite");
	Separator();
	if (m_pSprite)
		Button(m_pSprite->GetName().c_str());
	else
		Button("None");
	auto sprite = HandleSpriteDrop();
	if (sprite)
		m_pSprite = sprite;
	Separator();
}

void BubblePopBehaviour::Update(float elapsedSec)
{
	m_pSprite->Update(elapsedSec);
	if (m_pSprite)
	{
		if (m_pSprite->HasReachedEndOfSeq())
			SceneManager::GetInstance()->GetCurrentScene()->DeleteChild(m_pGameObject);
	}
}

void BubblePopBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));
}

void BubblePopBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	auto attribute = node->first_attribute("Sprite");
	if (attribute != 0)
		m_pSprite = m_pFSM->GetSprite(attribute->value());
}
