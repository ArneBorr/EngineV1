#pragma once
#include "Singleton.h"


struct SDL_Window;
struct SDL_Renderer;
class ImGuiWindows;

class Texture2D;
/**
	* Simple RAII wrapper for the SDL renderer
	*/
class Renderer final : public Singleton<Renderer>
{
public:
	void Initialize(SDL_Window* window);
	~Renderer();
	void Render() const;

	void RenderTexture(const Texture2D& texture, const Vector2f& pos, const Vector2f& scale = {1, 1}, float rot = 0, bool center = false) const;

	SDL_Renderer* GetSDLRenderer() const { return m_pRenderer; }
private:
	SDL_Renderer* m_pRenderer{ nullptr };
	ImGuiWindows* m_pImGuiWindows{ nullptr };
};


