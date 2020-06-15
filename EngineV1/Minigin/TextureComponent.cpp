#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "imgui.h"
#include "ResourceManager.h"
#include "SceneManager.h"

TextureComponent::TextureComponent(GameObject* pGameObject, const std::string& texturePath)
	: BaseComponent(pGameObject, "TextureComponent")
	, m_pTexture{ ResourceManager::GetInstance()->LoadTexture(texturePath) }
	, m_Path{ texturePath }
	, m_Offset{ }

{
	strcpy_s(m_TexturePathImGui, texturePath.c_str());
}

TextureComponent::~TextureComponent()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void TextureComponent::Render()
{
	if (!m_pTexture)
		return;

	const auto tranformComponent = m_pGameObject->GetTransform();
	if (!tranformComponent)
		return;

	const auto position = tranformComponent->GetWorldPosition();
	const auto scale = tranformComponent->GetWorldScale();
	const auto rot = tranformComponent->GetWorldRotation();

	Renderer::GetInstance()->RenderTexture(*m_pTexture, { position.x + m_Offset.x, position.y + m_Offset.y }, m_SrcRect, scale, rot, m_Center, m_Flip);
}

void TextureComponent::Update(float)
{
}

void TextureComponent::DrawInterface()
{
	using namespace ImGui;

	PushID(this);
	SetNextItemOpen(true, ImGuiCond_Once);
	bool open = TreeNode(&GetName().front());
	HandleDrag();

	if (open)
	{
		Separator();
		Spacing();

		//Path to texture
		Text("File Path");
		PushItemWidth(200);
		if (InputText(" ", m_TexturePathImGui, IM_ARRAYSIZE(m_TexturePathImGui)))
		{
			delete m_pTexture;
			m_pTexture = nullptr;
			m_pTexture = ResourceManager::GetInstance()->LoadTexture(m_TexturePathImGui);
			m_Path = m_TexturePathImGui;
		}

		Spacing();

		Text("Offset");
		PushItemWidth(150.f);
		InputFloat("x", &m_Offset.x, 1.f, 50.f, "%.1f");
		PushItemWidth(150.f);
		InputFloat("y", &m_Offset.y, 1.f, 50.f, "%.1f");

		Checkbox("Center", &m_Center);

		TreePop();
	}

	HandleDrop();

	PopID();
}

void TextureComponent::AdaptToFullscreen(const Vector2f&)
{
	m_Offset = SceneManager::GetInstance()->AdaptScaleToFullscreen(m_Offset);
}

void TextureComponent::SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode)
{
	pNode->append_attribute(pDoc->allocate_attribute("TexturePath", m_Path.c_str()));
	pNode->append_attribute(pDoc->allocate_attribute("OffsetX", FloatToXMLChar(pDoc, m_Offset.x)));
	pNode->append_attribute(pDoc->allocate_attribute("OffsetY", FloatToXMLChar(pDoc, m_Offset.y)));
	pNode->append_attribute(pDoc->allocate_attribute("Center", IntToXMLChar(pDoc, m_Center)));
}

void TextureComponent::SetAttributes(const Vector2f& offset, int center)
{
	m_Offset = offset;
	m_Center = center;
}

void TextureComponent::SetTexture(const std::string& texturePath)
{
	delete m_pTexture;
	m_pTexture = nullptr;

	m_Path = texturePath;
	m_pTexture = ResourceManager::GetInstance()->LoadTexture(texturePath);
}
