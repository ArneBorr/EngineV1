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
#include "ScriptComponent.h"
#include "Projectile.h"

AttackBehaviour::AttackBehaviour()
	: Behaviour("AttackBehaviour")
{
	m_IsOnTop = true;
}

AttackBehaviour::~AttackBehaviour()
{
	delete m_pProjectile;
	m_pProjectile = nullptr;
}

void AttackBehaviour::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf("ShootBehaviour::Initialize() : No Rigidbody Found");

	m_pFSM->GetBlackboard()->AddData("IsShooting", false);
}

void AttackBehaviour::Enter()
{
	bool left{ false };
	m_pFSM->GetBlackboard()->GetData("IsFacingLeft", left);
	m_pFSM->GetBlackboard()->SetData("IsShooting", true);
	m_IsShotFinished = false;

	//Handle Sprite
	if (m_pSprite)
	{
		m_pSprite->Reset();
		m_pSprite->Play(true);
		m_pSprite->Flip(left);
	}

	//Spawn Projectile
	auto pProjectile = GameObjectManager::GetInstance()->SpawnPrefab(m_pGameObject->GetScene(), m_pProjectile->GetName(), m_pGameObject->GetTransform()->GetPosition());

	if (pProjectile)
	{
		auto pRigidbody = pProjectile->GetComponent<RigidbodyComponent>();
		if (pRigidbody)
		{
			int sign = left ? -1 : 1;
			pRigidbody->MoveHorizontal({ 1.f * sign, 0.f });
		}

		FSMComponent* pFSMBubble = pProjectile->GetComponent<FSMComponent>();
		if (pFSMBubble)
		{
			pFSMBubble->GetBlackboard()->AddData("IsFacingLeft", left);
			pFSMBubble->Initialize();
		}
		else
		{
			auto scriptComp = pProjectile->GetComponent<ScriptComponent>();
			if (scriptComp)
			{
				auto script = static_cast<Projectile*>(scriptComp->GetScript());
				if (script)
					script->SetGoingLeft(left);
			}
			
		}

		
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
	Text("Projectile");
	Separator();

	if (m_pProjectile)
		Button(m_pProjectile->GetName());
	else
		Button("None");
	HandleProjectileDrop();
}

void AttackBehaviour::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));

	if (m_pProjectile)
		node->append_attribute(doc->allocate_attribute("Projectile", m_pProjectile->GetName()));
	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));
}

void AttackBehaviour::SetAttributes(rapidxml::xml_node<>* node)
{
	auto attribute = node->first_attribute("Projectile");
	if (attribute != 0)
		m_pProjectile = GameObjectManager::GetInstance()->GetPrefab(m_pGameObject->GetScene(), attribute->value());

	attribute = node->first_attribute("Sprite");
	if (attribute != 0)
		m_pSprite = m_pFSM->GetSprite(attribute->value());
}

void AttackBehaviour::SetTransitionsAndSprites(const std::vector<Behaviour*>&, const std::vector<Sprite*>& pSprites)
{
	if (pSprites.size() > 0)
		m_pSprite = pSprites[0];
}

void AttackBehaviour::HandleProjectileDrop()
{
	using namespace ImGui;
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Prefab"))
		{
			if (m_pProjectile)
				delete m_pProjectile;
			std::string name = *(static_cast<std::string*>(payload->Data));
			m_pProjectile = GameObjectManager::GetInstance()->GetPrefab(m_pGameObject->GetScene(), name);
		}

		ImGui::EndDragDropTarget();
	}
}
