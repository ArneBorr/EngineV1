#include "MiniginPCH.h"
#include "Effect2D.h"
#include <SDL_Mixer.h>

Mix_Chunk* Effect2D::GetSDLChunk() const
{
	return m_pChunk;
}

Effect2D::Effect2D(const std::string& path)
{
	m_pChunk = Mix_LoadWAV(path.c_str());
}

Effect2D::~Effect2D()
{
	Mix_FreeChunk(m_pChunk);
}
