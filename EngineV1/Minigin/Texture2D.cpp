#include "MiniginPCH.h"
#include "Texture2D.h"
#include <SDL.h>
#include "ResourceManager.h"


Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

Texture2D::Texture2D(const Texture2D& other) noexcept
{
	auto temp = ResourceManager::GetInstance()->LoadTexture(other.m_Name);
	m_pTexture = temp->m_pTexture;
	m_Name = other.m_Name;
}

SDL_Texture* Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

Texture2D::Texture2D(SDL_Texture* texture, const std::string& name)
	: m_Name{ name }
{
	m_pTexture = texture;
}
