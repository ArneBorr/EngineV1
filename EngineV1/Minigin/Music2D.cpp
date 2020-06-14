#include "MiniginPCH.h"
#include "Music2D.h"


Mix_Music* Music2D::GetSDLMusic() const
{
	return m_pMusic;
}

Music2D::Music2D(const std::string& path)
{
	m_pMusic = Mix_LoadMUS(path.c_str());
}

Music2D::~Music2D()
{
	Mix_FreeMusic(m_pMusic);
}