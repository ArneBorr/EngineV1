#include "MiniginPCH.h"
#include "BoxColliderComponent.h"
#include "imgui.h"
#include "RigidbodyComponent.h"
#include "Renderer.h"
#include <SDL.h>
#include "TransformComponent.h"
#include "ResourceManager.h"

BoxColliderComponent::BoxColliderComponent(GameObject* pObject, RigidbodyComponent* pBody)
	: BaseComponent(pObject, "BoxCollider"),
	m_pRigidbody{ pBody }
{
	if (pBody)
	{
		b2PolygonShape box;
		box.SetAsBox(m_Width, m_Height);
		pBody->ChangeShape(box);
	}
}

BoxColliderComponent::~BoxColliderComponent()
{
	
}

void BoxColliderComponent::Render()
{
	//auto SDLRenderer = Renderer::GetInstance()->GetSDLRenderer();
	auto transform = m_pGameObject->GetTransform();
	if (!transform)
		return;
	//SDL_SetRenderDrawColor(SDLRenderer, 1, 1, 1, 1);
	//SDL_Rect rect;
	////rect.x = int(transform->GetPosition().x - m_Width / 2.f);
	//rect.x = int(0);
	////rect.y = int(transform->GetPosition().y - m_Height / 2.f);
	//rect.y = int(5000);
	//rect.w = int(1000000);
	//rect.h = int(1000000);

	//SDL_RenderFillRect(SDLRenderer, nullptr);

	Renderer::GetInstance()->RenderTexture(*ResourceManager::GetInstance()->LoadTexture("BoxOutline.png"), transform->GetPosition(), { m_Width / 100.f, m_Height / 100.f });
}

void BoxColliderComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void BoxColliderComponent::DrawInterface()
{
	using namespace ImGui;
	SetNextItemOpen(true, ImGuiCond_Once);

	bool open = TreeNode(&GetName().front());
	HandleDrag();

	if (open)
	{
		Separator();
		Spacing();

		Text("BoxCollider");

		if (ImGui::InputFloat("Width", &m_Width))
		{
			CreateShape();
		}

		if (ImGui::InputFloat("Height", &m_Height))
		{
			CreateShape();
		}

		TreePop();
	}

	HandleDrop();
}

void BoxColliderComponent::SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node)
{
	UNREFERENCED_PARAMETER(doc);
	UNREFERENCED_PARAMETER(node);
}

void BoxColliderComponent::CreateLink(RigidbodyComponent* pBody)
{
	m_pRigidbody = pBody;
	CreateShape();
}

void BoxColliderComponent::CreateShape()
{
	if (m_pRigidbody)
	{
		b2PolygonShape box;
		box.SetAsBox(m_Width, m_Height);
		m_pRigidbody->ChangeShape(box);
	}
}
