#include "MiniginPCH.h"
#include "BubbleHitEnemyBehaviour.h"
#include "Sprite.h"
#include "FSMComponent.h"

BubbleHitEnemyBehaviour::BubbleHitEnemyBehaviour()
	: Behaviour("BubbleHitEnemyBehaviour")
{
}

void BubbleHitEnemyBehaviour::Enter()
{
}

Behaviour* BubbleHitEnemyBehaviour::HandleInput()
{
	return nullptr;
}

void BubbleHitEnemyBehaviour::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void BubbleHitEnemyBehaviour::DrawInterface()
{
}

void BubbleHitEnemyBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	node->append_attribute(doc->allocate_attribute("NrOfTransitions", IntToXMLChar(doc, 0)));

	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));
}

void BubbleHitEnemyBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	std::vector<std::string> transitions;
	std::string sprite{ "" };
	GetTransitionsAndSpriteFromAtrribute(transitions, node, sprite);

	if (sprite != "")
		m_pSprite = m_pFSM->GetSprite(sprite);
}
