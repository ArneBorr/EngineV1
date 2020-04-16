#pragma once

struct SDL_Window;
class MainGame
{
public:
	void Initialize();
	void LoadGame() const;
	void Cleanup();
	void Run();
private:
	static const float m_MsPerUpdate; //0.016 for 60 fps, 0.033 for 30 fps
	SDL_Window* m_Window{};
	
};

