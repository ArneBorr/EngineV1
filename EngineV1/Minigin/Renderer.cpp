#include "MiniginPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"
#include "imgui.h"
#include "imgui_sdl.h"
#include "Scene.h"
#include "ImGuiWindows.h"


void Renderer::Initialize(SDL_Window* window)
{
	m_pRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_pRenderer == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
	ImGui::CreateContext();
	auto windowSize = GameInfo::GetInstance()->GetWindowSize();
	ImGuiSDL::Initialize(m_pRenderer, int(windowSize.x), int(windowSize.y));

	m_pImGuiWindows = new ImGuiWindows{};

	//https://github.com/ocornut/imgui/issues/707 Theme
	static int hue = 140;
	static float col_main_sat = 180.f / 255.f;
	static float col_main_val = 161.f / 255.f;
	static float col_area_sat = 124.f / 255.f;
	static float col_area_val = 100.f / 255.f;
	static float col_back_sat = 59.f / 255.f;
	static float col_back_val = 40.f / 255.f;

	ImGuiStyle& style = ImGui::GetStyle();

	ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
	ImVec4 col_main = ImColor::HSV(hue / 255.f, col_main_sat, col_main_val);
	ImVec4 col_back = ImColor::HSV(hue / 255.f, col_back_sat, col_back_val);
	ImVec4 col_area = ImColor::HSV(hue / 255.f, col_area_sat, col_area_val);

	style.Colors[ImGuiCol_Text] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(col_text.x, col_text.y, col_text.z, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(col_back.x, col_back.y, col_back.z, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(col_text.x, col_text.y, col_text.z, 0.30f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.68f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.45f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(col_main.x, col_main.y, col_main.z, 0.35f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 0.57f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(col_main.x, col_main.y, col_main.z, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.24f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(col_main.x, col_main.y, col_main.z, 0.44f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(col_main.x, col_main.y, col_main.z, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

Renderer::~Renderer()
{
	ImGuiSDL::Deinitialize();
	ImGui::DestroyContext();

	if (m_pRenderer != nullptr)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}

	delete m_pImGuiWindows;
	m_pImGuiWindows = nullptr;
}

void Renderer::Render() const
{
	SDL_RenderClear(m_pRenderer);
	ImGui::NewFrame();
	
	SceneManager::GetInstance()->Render();

	if (!GameInfo::GetInstance()->IsFullscreen())
		m_pImGuiWindows->Render();	

	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());

	SDL_RenderPresent(m_pRenderer);

}

void Renderer::RenderTexture(const Texture2D& texture, const Vector2f& pos, const Vector4f& srcRect, const Vector2f& scale, float rot, bool center, bool flip) const //srcRect x/y = Pos, z = width, w - height
{
	SDL_Rect dst;

	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);

	const float width = float(dst.w) * scale.x;
	const float height = float(dst.h) * scale.y;

	dst.w = int(width);
	dst.h = int(height);
	
	SDL_Rect src;
	bool useSrcRect = false;
	if (srcRect.z != 0 && srcRect.w != 0)
	{
		src.x = int(srcRect.x); //XPos
		src.y = int(srcRect.y); //YPOS
		src.w = int(srcRect.z); //Width
		src.h = int(srcRect.w); //Height
		dst.w = int(srcRect.z * scale.x); //Width Dest Rect
		dst.h = int(srcRect.w * scale.y); //Height Dest Rect
		useSrcRect = true;
	}

	if (center)
	{
		dst.x = int(pos.x - dst.w / 2);
		dst.y = int(pos.y - dst.h / 2);
	}
	else
	{
		dst.x = int(pos.x);
		dst.y = int(pos.y);
	}

	SDL_RendererFlip flipFlag = SDL_RendererFlip::SDL_FLIP_NONE;
	if (flip)
		flipFlag = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;

	if (useSrcRect)
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst, rot, nullptr, flipFlag);
	else
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, rot, nullptr, flipFlag);
}

void Renderer::RenderLine(const Vector4f& points)
{
	SDL_SetRenderDrawColor(GetSDLRenderer(), 255, 0, 0, 255);
	SDL_RenderDrawLine(GetSDLRenderer(), int(points.x), int(points.y), int(points.z), int(points.w));
	SDL_SetRenderDrawColor(GetSDLRenderer(), 0, 0, 0, 255);
}

void Renderer::RenderSquare(const Vector4f& pointsLeft, const Vector4f& pointsRight)
{
	// PointsLeft: x/y leftTop, z/w botLeft ; PointsRight: x/y rightTop, z/w botRight

	RenderLine({ pointsLeft.z, pointsLeft.w, pointsLeft.x, pointsLeft.y }); // Botleft to TopLeft
	RenderLine({ pointsLeft.x, pointsLeft.y, pointsRight.x, pointsRight.y }); // Topleft to TopRight
	RenderLine({ pointsRight.x, pointsRight.y, pointsRight.z, pointsRight.w }); // TopRight to BotRight
	RenderLine({ pointsRight.z, pointsRight.w, pointsLeft.z, pointsLeft.w }); // BotRight to BotLeft
}

const Vector4f& Renderer::GetEditorDimensions()
{
	return m_pImGuiWindows->GetEditorDimensions();
}
