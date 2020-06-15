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

	//srcRect x/y = Pos, z = width, w - height
	void RenderTexture(const Texture2D& texture, const Vector2f& pos, const Vector4f& srcRect = {}, const Vector2f& scale = { 1, 1 }, float rot = 0, bool center = false, bool flip = false) const;
	void RenderLine(const Vector4f& points);
	void RenderSquare(const Vector4f& pointsLeft, const Vector4f& pointsRight); // PointsLeft: x/y leftTop, z/w botLeft ; PointsRight: x/y rightTop, z/w botRight

	SDL_Renderer* GetSDLRenderer() const { return m_pRenderer; }
	const Vector4f& GetEditorDimensions();
private:
	SDL_Renderer* m_pRenderer = nullptr;
	ImGuiWindows* m_pImGuiWindows = nullptr;
};


