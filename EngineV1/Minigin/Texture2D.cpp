#include "MiniginPCH.h"
#include "Texture2D.h"
#include <SDL.h>

Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

SDL_Texture* Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

Texture2D::Texture2D(SDL_Texture* texture)
{
	m_pTexture = texture;
}
