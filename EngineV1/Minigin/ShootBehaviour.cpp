#include "MiniginPCH.h"
#include "ShootBehaviour.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Sprite.h"
#include "imgui.h"
#include "FSMComponent.h"

ShootBehaviour::ShootBehaviour()
	: Behaviour("ShootBehaviour")
{
	m_IsOnTop = true;
}

void ShootBehaviour::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf("ShootBehaviour::Initialize() : No Rigidbody Found");
}

void ShootBehaviour::Enter()
{
	m_pSprite->Reset();
	m_pSprite->Play(true);
	m_IsShotFinished = false;
}

Behaviour* ShootBehaviour::HandleInput()
{
	if (m_IsShotFinished)
		return nullptr;
	else
		return this;
}

void ShootBehaviour::Update(float elapsedSec)
{
	//Handle Sprite
	if (m_pRigidbody->GetVelocity().x < 0)
		m_pSprite->Flip(true);
	else
		m_pSprite->Flip(false);

	m_pSprite->Update(elapsedSec);

	if (m_pSprite->HasReachedEndOfSeq())
		m_IsShotFinished = true;
}

void ShootBehaviour::DrawInterface()
{
	using namespace ImGui;
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

void ShootBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	node->append_attribute(doc->allocate_attribute("NrOfTransitions", IntToXMLChar(doc, 0)));
	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));
}

void ShootBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	std::vector<std::string> transitions;
	std::string sprite{ "" };
	GetTransitionsAndSpriteFromAtrribute(transitions, node, sprite);

	if (sprite != "")
		m_pSprite = m_pFSM->GetSprite(sprite);
}
