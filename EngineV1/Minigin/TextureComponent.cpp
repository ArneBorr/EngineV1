#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "imgui.h"


TextureComponent::TextureComponent(GameObject* pGameObject, const std::string& texture)
	: BaseComponent(pGameObject, "TextureComponent")
	, m_pTexture(ResourceManager::GetInstance()->LoadTexture(texture))
{
}

void TextureComponent::Render()
{
	if (!m_pTexture)
		return; // LOG ERROR

	const auto tranformComponent = m_pGameObject->GetComponent<TransformComponent>();
	if (!tranformComponent)
		return; // ERROR LOG

	const auto position = tranformComponent->GetPosition();

	Renderer::GetInstance()->RenderTexture(*m_pTexture, position.x + m_Offset.x, position.y + m_Offset.y);
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
		SameLine();
		InputFloat("z", &m_Offset.z);
		PopItemWidth();


		TreePop();
	}

	HandleDrop();
}

void TextureComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance()->LoadTexture(filename);
}

void TextureComponent::SetPosition(glm::vec3 offset)
{
	m_Offset = offset;
}
