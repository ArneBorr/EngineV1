#include "MiniginPCH.h"
#include "AttackBehaviour.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Sprite.h"
#include "imgui.h"
#include "FSMComponent.h"
#include "Blackboard.h"
#include "Scene.h"
#include "TransformComponent.h"

AttackBehaviour::AttackBehaviour()
	: Behaviour("AttackBehaviour")
{
	m_IsOnTop = true;
}

void AttackBehaviour::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf("ShootBehaviour::Initialize() : No Rigidbody Found");

	m_pFSM->GetBlackboard()->AddData("IsShooting", true);
}

void AttackBehaviour::Enter()
{
	bool isFacingLeft = false;
	m_pFSM->GetBlackboard()->GetData("IsFacingLeft", isFacingLeft);
	m_pFSM->GetBlackboard()->SetData("IsShooting", true);

	//Handle Sprite
	if (m_pSprite)
	{
		m_pSprite->Reset();
		m_pSprite->Play(true);
		m_pSprite->Flip(isFacingLeft);
	}

	m_IsShotFinished = false;


	//Spawn Bubble
	auto pBubble = GameObjectManager::GetInstance()->SpawnPrefab("Bubble", m_pGameObject->GetTransform()->GetPosition());
	FSMComponent* pFSMBubble = pBubble->GetComponent<FSMComponent>();
	if (pFSMBubble)
	{
		pFSMBubble->GetBlackboard()->AddData("IsFacingLeft", isFacingLeft);
		pFSMBubble->Initialize();
	}

}

Behaviour* AttackBehaviour::HandleInput()
{
	if (m_IsShotFinished)
		return nullptr;
	else
		return this;
}

void AttackBehaviour::Update(float elapsedSec)
{
	//Handle Sprite
	m_pSprite->Update(elapsedSec);

	if (m_pSprite->HasReachedEndOfSeq())
		m_IsShotFinished = true;
}

void AttackBehaviour::Exit()
{
	m_pFSM->GetBlackboard()->SetData("IsShooting", false);
}

void AttackBehaviour::DrawInterface()
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

void AttackBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	node->append_attribute(doc->allocate_attribute("NrOfTransitions", IntToXMLChar(doc, 0)));
	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));
}

void AttackBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	std::vector<std::string> transitions;
	std::string sprite{ "" };
	GetTransitionsAndSpriteFromAtrribute(transitions, node, sprite);

	if (sprite != "")
		m_pSprite = m_pFSM->GetSprite(sprite);
}