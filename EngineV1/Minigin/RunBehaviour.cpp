#include "MiniginPCH.h"
#include "RunBehaviour.h"
#include "Sprite.h"
#include "FSMComponent.h"

RunBehaviour::RunBehaviour()
	: Behaviour("RunBehaviour")
{
}

void RunBehaviour::Enter()
{
}

Behaviour* RunBehaviour::HandleInput()
{
	return nullptr;
}

void RunBehaviour::Update(float elapsesSec)
{
	UNREFERENCED_PARAMETER(elapsesSec);
}

void RunBehaviour::Render()
{
}

void RunBehaviour::Exit()
{
}

void RunBehaviour::DrawInterface()
{
}

void RunBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	node->append_attribute(doc->allocate_attribute("NrOfTransitions", IntToXMLChar(doc, 0)));
	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));
}

void RunBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	std::vector<std::string> transitions;
	std::string sprite{ "" };
	GetTransitionsAndSpriteFromAtrribute(transitions, node, sprite);

	if (sprite != "")
		m_pSprite = m_pFSM->GetSprite(sprite);
}
