#pragma once

struct SDL_Window;
class MainGame
{
public:
	MainGame();
	void Initialize();
	void LoadGame() const;
	void Cleanup();
	void Run();
private:
	static const float m_MsPerUpdate; 
	SDL_Window* m_Window{};
};

