#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"

TextureComponent::TextureComponent(GameObject* pGameObject, const std::string& texture)
	: BaseComponent(pGameObject)
	, m_pTexture(ResourceManager::GetInstance()->LoadTexture(texture))
{
}

void TextureComponent::Render()
{
	const auto position = m_pGameObject->GetTransform().GetPosition();
	Renderer::GetInstance()->RenderTexture(*m_pTexture, position.x + m_Offset.x, position.y + m_Offset.y);
}

void TextureComponent::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);
}

void TextureComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance()->LoadTexture(filename);
}

void TextureComponent::SetPosition(glm::vec3 offset)
{
	m_Offset = offset;
}
