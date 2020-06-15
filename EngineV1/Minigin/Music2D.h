#pragma once
#include <SDL_mixer.h>

class Music2D
{
public:
	Mix_Music* GetSDLMusic() const;
	Music2D(const std::string& path);
	~Music2D();

	Music2D(const Music2D&) = delete;
	Music2D(Music2D&&) = delete;
	Music2D& operator= (const Music2D&) = delete;
	Music2D& operator= (const Music2D&&) = delete;

private:
	Mix_Music* m_pMusic = nullptr;
};

