#include "MiniginPCH.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "imgui.h"

TextComponent::TextComponent(GameObject* pGameObject, Font* pFont, const std::string& text)
	: BaseComponent(pGameObject, "TextComponent")
	, m_pTexture{nullptr}
	, m_pFont{ pFont }
	, m_Text{text}
	, m_InterfaceText{ *text.c_str() }
{
	if (!m_pFont)
		m_pFont = ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36);
	UpdateTexture();
}

void TextComponent::Render()
{
	if (!m_pTexture)
	{
		return; // LOG ERROR
	}

	const auto tranformComponent = m_pGameObject->GetComponent<TransformComponent>();
	if (!tranformComponent)
		return; // ERROR LOG

	const auto position = tranformComponent->GetPosition();

	Renderer::GetInstance()->RenderTexture(*m_pTexture, position.x + m_Offset.x, position.y + m_Offset.y);
}

void TextComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void TextComponent::DrawInterface()
{
	using namespace ImGui;
	if (TreeNode(&GetName().front()))
	{
		Separator();
		Spacing();


		Text("Text");
		ImGui::InputText("Text", m_InterfaceText, 128);
		ImGui::SameLine();
		if (ImGui::Button("Update"))
		{
			SetText(m_InterfaceText);
		}

		Spacing();

		Text("Offset");
		PushItemWidth(50.f);
		InputFloat("x", &m_Offset.x);
		SameLine();
		InputFloat("y", &m_Offset.y);
		SameLine();
		InputFloat("z", &m_Offset.z);
		PopItemWidth();

		TreePop();
	}
	
}

void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	UpdateTexture();
}

void TextComponent::SetPosition(glm::vec3 pos)
{
	m_Offset = pos;
}

void TextComponent::UpdateTexture()
{
	const SDL_Color color = { 255,255,255 }; // only white text is supported now
	const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance()->GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	m_pTexture = new Texture2D(texture);
}
