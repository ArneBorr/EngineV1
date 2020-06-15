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
#include "ResourceManager.h"


TextComponent::TextComponent(GameObject* pGameObject, Font* pFont, const std::string& text)
	: BaseComponent(pGameObject, "TextComponent")
	, m_pTexture{nullptr}
	, m_pFont{ pFont }
	, m_Text{text}
{
	if (!m_pFont)
		m_pFont = ResourceManager::GetInstance()->LoadFont("Lingua.otf", 36);
	UpdateTexture();
}

TextComponent::~TextComponent()
{
	delete m_pTexture;
	m_pTexture = nullptr;

	delete m_pFont;
	m_pFont = nullptr;
}

void TextComponent::Render()
{
	if (!m_pTexture)
		return;

	const auto pTranformComponent = m_pGameObject->GetTransform();
	if (!pTranformComponent)
		return;

	const auto position = pTranformComponent->GetWorldPosition();
	const auto scale = pTranformComponent->GetWorldScale();
	const auto rot = pTranformComponent->GetWorldRotation();

	Renderer::GetInstance()->RenderTexture(*m_pTexture, { position.x + m_Offset.x, position.y + m_Offset.y }, {}, scale, rot);
}

void TextComponent::Update(float)
{
}

void TextComponent::DrawInterface()
{
	using namespace ImGui;
	SetNextItemOpen(true, ImGuiCond_Once);

	PushID(this);
	bool open = TreeNode(&GetName().front());
	HandleDrag();

	if (open)
	{
		Separator();
		Spacing();

		//Text to render
		Text("Text");
		if (InputText("Text", &m_Text.front(), 128))
		{
			UpdateTexture();
		}
		SameLine();
		Spacing();

		//Offset in regard to transform component
		Text("Offset");
		PushItemWidth(100.f);
		InputFloat("x", &m_Offset.x, 1.f, 50.f, "%.1f");
		InputFloat("y", &m_Offset.y, 1.f, 50.f, "%.1f");
		PopItemWidth();

		TreePop();
	}

	HandleDrop();
	PopID();
}

void TextComponent::SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode)
{
	pNode->append_attribute(pDoc->allocate_attribute("FontPath", m_pFont->GetPath().c_str()));
	pNode->append_attribute(pDoc->allocate_attribute("FontSize", IntToXMLChar(pDoc, m_pFont->GetSize())));
	pNode->append_attribute(pDoc->allocate_attribute("Text", m_Text.c_str()));
	pNode->append_attribute(pDoc->allocate_attribute("OffsetX", FloatToXMLChar(pDoc, m_Offset.x)));
	pNode->append_attribute(pDoc->allocate_attribute("OffsetY", FloatToXMLChar(pDoc, m_Offset.y)));
}

void TextComponent::SetAttributes(const Vector2f& offset)
{
	m_Offset = offset;
}

void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	UpdateTexture();
}

void TextComponent::UpdateTexture()
{
	delete m_pTexture;
	const SDL_Color color = { 255,255,255 };

	if (m_Text.front() == '\0')
		m_Text = " ";

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
