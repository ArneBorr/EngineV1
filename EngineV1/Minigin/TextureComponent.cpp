#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "imgui.h"
#include "ResourceManager.h"


TextureComponent::TextureComponent(GameObject* pGameObject, const std::string& texture)
	: BaseComponent(pGameObject, "TextureComponent")
	, m_pTexture{ ResourceManager::GetInstance()->LoadTexture(texture) }
	, m_Path{ texture }
	, m_Offset{}
{
}

TextureComponent::~TextureComponent()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void TextureComponent::Render()
{
	if (!m_pTexture)
		return; // LOG ERROR

	const auto tranformComponent = m_pGameObject->GetTransform();
	if (!tranformComponent)
		return; // ERROR LOG

	const auto position = tranformComponent->GetWorldPosition();
	const auto scale = tranformComponent->GetWorldScale();
	const auto rot = tranformComponent->GetWorldRotation();

	Renderer::GetInstance()->RenderTexture(*m_pTexture, { position.x + m_Offset.x, position.y + m_Offset.y }, scale, rot, m_Center, m_SrcRect);
}

void TextureComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void TextureComponent::DrawInterface()
{
	using namespace ImGui;

	SetNextItemOpen(true, ImGuiCond_Once);

	bool open = TreeNode(&GetName().front());
	HandleDrag();

	if (open)
	{
		Separator();
		Spacing();

		static char texturepath[128] = " ";
		Text("TextureFile");
		ImGui::InputText("Texturepath", texturepath, 128);
		ImGui::SameLine();
		if (ImGui::Button("Update"))
		{
			m_pTexture = ResourceManager::GetInstance()->LoadTexture(texturepath);
		}


		Spacing();

		Text("Offset");
		PushItemWidth(50.f);
		InputFloat("x", &m_Offset.x);
		SameLine();
		InputFloat("y", &m_Offset.y);

		Checkbox("Center", &m_Center);

		TreePop();
	}

	HandleDrop();
}

void TextureComponent::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("TexturePath", m_Path.c_str()));
	node->append_attribute(doc->allocate_attribute("OffsetX", FloatToXMLChar(doc, m_Offset.x)));
	node->append_attribute(doc->allocate_attribute("OffsetY", FloatToXMLChar(doc, m_Offset.y)));
	node->append_attribute(doc->allocate_attribute("Center", IntToXMLChar(doc, m_Center)));
}

void TextureComponent::SetAttributes(const Vector2f& offset, int center)
{
	m_Offset = offset;
	m_Center = center;
}

void TextureComponent::SetTexture(const std::string& texture)
{
	delete m_pTexture;
	m_pTexture = nullptr;

	m_Path = texture;
	m_pTexture = ResourceManager::GetInstance()->LoadTexture(texture);
}
