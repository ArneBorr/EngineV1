#include "MiniginPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"
#include "imgui.h"
#include "imgui_sdl.h"
#include "Scene.h"



void Renderer::Init(SDL_Window* window)
{
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
	ImGui::CreateContext();
	auto windowSize = GameInfo::GetInstance()->GetWindowSize();
	ImGuiSDL::Initialize(m_Renderer, int(windowSize.x), int(windowSize.y));

	//ImGui::Windowfl
	ImGui::GetStyle().Alpha = 1.f;
	//ImGui::GetStyle().Colors = ImGuiStyle::Colors();
}

void Renderer::Render() const
{
	SDL_RenderClear(m_Renderer);
	ImGui::NewFrame();
	

	SceneManager::GetInstance()->Render();

	if (!GameInfo::GetInstance()->IsFullscreen())
	{
		SceneManager::GetInstance()->GetCurrentScene()->DrawInterface();
		SceneManager::GetInstance()->DrawInterface();
		GameInfo::GetInstance()->DrawInterface();
		GameObjectManager::GetInstance()->DrawInterface();
	}
	

	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());

	SDL_RenderPresent(m_Renderer);

}

void Renderer::Destroy()
{
	ImGuiSDL::Deinitialize();
	ImGui::DestroyContext();

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float widthScale, const float heightScale) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);

	int tempW = 0;
	int tempH = 0;

	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &tempW, &tempH);
	
	const float width = static_cast<float>(tempW) * widthScale;
	const float height = static_cast<float>(tempH) * heightScale;

	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}
