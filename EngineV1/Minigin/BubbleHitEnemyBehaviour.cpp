#include "MiniginPCH.h"
#include "BubbleHitEnemyBehaviour.h"
#include "Sprite.h"
#include "FSMComponent.h"
#include "Blackboard.h"

BubbleHitEnemyBehaviour::BubbleHitEnemyBehaviour()
	: Behaviour("BubbleHitEnemyBehaviour")
{
}

void BubbleHitEnemyBehaviour::Initialize()
{
	
}

void BubbleHitEnemyBehaviour::Enter()
{
}

Behaviour* BubbleHitEnemyBehaviour::HandleInput()
{
	return nullptr;
}

void BubbleHitEnemyBehaviour::Update(float)
{
}

void BubbleHitEnemyBehaviour::OnNotify(const std::string&, GameObject*)
{
}

void BubbleHitEnemyBehaviour::DrawInterface()
{
}

void BubbleHitEnemyBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));
}

void BubbleHitEnemyBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	auto attribute = node->first_attribute("Sprite");
	if (attribute != 0)
		m_pSprite = m_pFSM->GetSprite(attribute->value());
}
