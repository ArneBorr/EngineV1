#include "MiniginPCH.h"
#include "LaunchEnemy.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Sprite.h"
#include "imgui.h"
#include "SceneManager.h"
#include "Scene.h"
#include "FSMComponent.h"

LaunchEnemy::LaunchEnemy()
	: Behaviour("LaunchEnemy")
{
}

void LaunchEnemy::Initialize()
{
	m_pRigidbody = m_pGameObject->GetRigidbody();
	if (!m_pRigidbody)
		std::printf("IdleBehaviour::Initialize() : No rigidbody found!\n");
}

void LaunchEnemy::Enter()
{
	if (m_pRigidbody)
	{
		m_pRigidbody->SetFixedRotatoon(false);
		m_pRigidbody->SetRestitution(0.5f);

		bool left = std::rand() % 2;
		int sign = left ? -1 : 1;
		m_pRigidbody->MoveHorizontal({ m_Speed, 0 });
		m_pRigidbody->MoveVertical(m_Speed * sign);
		m_pRigidbody->SetAngularVelocity(m_Speed * sign);
	}
	if (m_pSprite)
		m_pSprite->Play(true);
}

Behaviour* LaunchEnemy::HandleInput()
{
	return nullptr;
}

void LaunchEnemy::Update(float elapsedSec)
{
	m_Timer += elapsedSec;

	if (m_pRigidbody)
	{
		if (m_pRigidbody->IsOnGround() && abs(m_pRigidbody->GetVelocity().y) < 0.05f)
		{
			GameObjectManager::GetInstance()->SpawnPrefab("Fries", m_pRigidbody->GetPosition());
			SceneManager::GetInstance()->GetCurrentScene()->DeleteChild(m_pGameObject);
		}
		if (m_Timer > m_MaxRotateTime)
		{
			m_pRigidbody->SetRotation(0);
			m_pRigidbody->SetFixedRotatoon(true);
		}
	}

	if (m_pSprite)
		m_pSprite->Update(elapsedSec);
}

void LaunchEnemy::DrawInterface()
{
	using namespace ImGui;
	Separator();
	Text("Transitions");
	Separator();

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

	//Constants
	Text("Constants");
	Separator();
	PushItemWidth(40.f);
	InputFloat("Speed", &m_Speed);
	Separator();
}

void LaunchEnemy::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name.c_str()));
	if (m_pSprite)
		node->append_attribute(doc->allocate_attribute("Sprite", m_pSprite->GetNameRef()));

	node->append_attribute(doc->allocate_attribute("Speed", FloatToXMLChar(doc, m_Speed)));
}

void LaunchEnemy::SetAttributes(rapidxml::xml_node<>* node)
{
	auto attribute = node->first_attribute("Sprite");
	if (attribute != 0)
		m_pSprite = m_pFSM->GetSprite(attribute->value());

	m_Speed = std::stof(node->first_attribute("Speed")->value());
}

void LaunchEnemy::SetTransitionsAndSprites(const std::vector<Behaviour*>&, const std::vector<Sprite*>& pSprites)
{
	if (pSprites.size() == 1)
		m_pSprite = pSprites[0];
}
