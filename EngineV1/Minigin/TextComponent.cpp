#include "MiniginPCH.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"

TextComponent::TextComponent(std::weak_ptr<GameObject> pGameObject, const std::shared_ptr<Font>& pFont, const std::string& text)
	: BaseComponent(pGameObject)
	, m_Texture{nullptr}
	, m_pFont{ pFont }
	, m_Text{text}
{
	UpdateTexture();
}

void TextComponent::Render()
{
	if (m_Texture)
	{
		const auto pos = m_pGameObject.lock()->GetTransform().GetPosition();
		Renderer::GetInstance()->RenderTexture(*m_Texture, pos.x + m_Offset.x, pos.y + m_Offset.y);
	}
}

void TextComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
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
	m_Texture = std::make_shared<Texture2D>(texture);
}
