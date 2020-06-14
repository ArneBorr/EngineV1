#include "MiniginPCH.h"
#include "AnimatorControllerComponent.h"
#include "imgui.h"
#include "TextureComponent.h"

AnimatorControllerComponent::AnimatorControllerComponent(GameObject* pObject)
	: BaseComponent(pObject, "AnimatorController")
{
}

AnimatorControllerComponent::~AnimatorControllerComponent()
{
	for (auto sprite : m_pSprites)
	{
		delete sprite;
		sprite = nullptr;
	}
	m_pSprites.clear();
}

void AnimatorControllerComponent::Render()
{
	if (m_pSprites.size() > 0)
		m_pSprites[m_ActiveSprite]->Render();
}

void AnimatorControllerComponent::Update(float elapsedSec)
{
	if (m_IsPlaying)
	{
		if (m_pSprites.size() > 0)
			m_pSprites[m_ActiveSprite]->Update(elapsedSec);
	}	
}

void AnimatorControllerComponent::DrawInterface()
{
	using namespace ImGui;
	SetNextItemOpen(true, ImGuiCond_Once);

	bool open = TreeNode(&GetName().front());
	HandleDrag();

	if (open)
	{
		Separator();
		
		if (Button("Window"))
			m_IsWindowOpen = !m_IsWindowOpen;

		if (m_IsWindowOpen)
		{
			SetNextWindowBgAlpha(1.f);
			SetNextWindowPos({ 200, 200 }, ImGuiCond_Appearing);
			SetNextWindowSize({ 500, 500 }, ImGuiCond_Appearing);
			Begin("Animation Controller");
			Text("Sprites");
			Separator();

			//Add sprites
			static char m_ToBeAddedSpriteName[20]{ "" };
			InputText(" ", m_ToBeAddedSpriteName, IM_ARRAYSIZE(m_ToBeAddedSpriteName));
			SameLine();
			if (Button("AddSprite"))
				m_pSprites.push_back(new Sprite{ m_pGameObject, m_ToBeAddedSpriteName });

			//Draw Sprites
			unsigned int i{};
			for (auto it = m_pSprites.begin(); it != m_pSprites.end();)
			{
				PushID(&(*it));
				(*it)->DrawInterface();


				if (Button("Set default"))
					m_ActiveSprite = i;

				if (m_ActiveSprite == i)
				{
					SameLine();
					Text("Is Default");
				}

				if (Button("Delete Sprite"))
					it = m_pSprites.erase(it);
				else
					++it;

				PopID();
				Separator();
				i++;
			}
			End();
		
		}

		TreePop();
	}

	HandleDrop();
}

void AnimatorControllerComponent::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	for (auto sprite : m_pSprites)
	{
		rapidxml::xml_node<>* spriteNode = doc->allocate_node(rapidxml::node_element, "Sprite");
		sprite->SaveAttributes(doc, spriteNode);
		node->append_node(spriteNode);
	}
}

void AnimatorControllerComponent::SetAttributes(const std::vector<Sprite*>& sprites)
{
	m_pSprites = sprites;
}
