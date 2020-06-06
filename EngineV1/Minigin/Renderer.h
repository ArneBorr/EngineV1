#pragma once
#include "Singleton.h"


struct SDL_Window;
struct SDL_Renderer;

class Texture2D;
/**
	* Simple RAII wrapper for the SDL renderer
	*/
class Renderer final : public Singleton<Renderer>
{
public:
	void Init(SDL_Window* window);
	void Render() const;
	void Destroy();

	void RenderTexture(const Texture2D& texture, const Vector2f& pos, const Vector2f& scale = {}, float rot = 0) const;

	SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
private:
	SDL_Renderer* m_Renderer{};
};


