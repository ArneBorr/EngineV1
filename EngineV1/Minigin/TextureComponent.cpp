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

	const auto position = tranformComponent->GetPosition();
	const auto scale = tranformComponent->GetScale();
	const auto rot = tranformComponent->GetRotation();

	Renderer::GetInstance()->RenderTexture(*m_pTexture, { position.x + m_Offset.x, position.y + m_Offset.y }, scale, rot);
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

		TreePop();
	}

	HandleDrop();
}

void TextureComponent::SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc.allocate_attribute("TexturePath", m_Path.c_str()));
	node->append_attribute(doc.allocate_attribute("OffsetX", FloatToXMLChar(doc, m_Offset.x)));
	node->append_attribute(doc.allocate_attribute("OffsetY", FloatToXMLChar(doc, m_Offset.y)));
}

void TextureComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance()->LoadTexture(filename);
}

void TextureComponent::SetPosition(float x, float y)
{
	m_Offset.x = x;
	m_Offset.y = y;
}

void TextureComponent::SetPosition(const Vector2f& offset)
{
	SetPosition(offset.x, offset.y);
}
