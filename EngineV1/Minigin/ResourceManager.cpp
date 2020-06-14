#include "MiniginPCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "Music2D.h"
#include "Effect2D.h"

void ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	m_DefaultTexturePath = dataPath + "MissingTexture.png";

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG)
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

Texture2D* ResourceManager::LoadTexture(const std::string& file) const
{
	const auto fullPath = m_DataPath + file;
	auto texture = IMG_LoadTexture(Renderer::GetInstance()->GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		texture = IMG_LoadTexture(Renderer::GetInstance()->GetSDLRenderer(), m_DefaultTexturePath.c_str());
	}
	return new Texture2D(texture);
}

Font* ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return new Font(m_DataPath + file, size);
}

Music2D* ResourceManager::LoadMusic(const std::string& path)
{
	return new Music2D(m_DataPath + path);
}

Effect2D* ResourceManager::LoadSoundEffect(const std::string& path)
{
	return new Effect2D(m_DataPath + path);
}
