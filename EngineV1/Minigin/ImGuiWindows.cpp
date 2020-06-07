#include "MiniginPCH.h"
#include "ImGuiWindows.h"
#include "GameObjectManager.h"
#include "imgui.h"
#include "SceneManager.h"

using namespace ImGui;

ImGuiWindows::ImGuiWindows()
{
	const Vector2f windowSize{ GameInfo::GetInstance()->GetWindowSize() };

	static float widthRatioVertical = 0.2f;
	static float heightRatioVertical = 1.f;
	static float widthRatioHorizontal = 0.6f;
	static float heightRatioTop = 0.14f;
	const float heightRatioBottom{ 0.26f };

	m_WindowPositions[0] = { 0,0 };
	m_WindowSizes[0] = { windowSize.x * widthRatioVertical, windowSize.y * heightRatioVertical };

	m_WindowPositions[1] = { windowSize.x * widthRatioVertical, 0 };
	m_WindowSizes[1] = { windowSize.x * widthRatioHorizontal, windowSize.y * heightRatioTop };

	m_WindowPositions[2] = { windowSize.x - windowSize.x * widthRatioVertical, 0 };
	m_WindowSizes[2] = { windowSize.x * widthRatioVertical, windowSize.y * heightRatioVertical };


	m_WindowPositions[3] = { windowSize.x * widthRatioVertical, windowSize.y - windowSize.y * heightRatioBottom };
	m_WindowSizes[3] = { windowSize.x * widthRatioHorizontal, windowSize.y * heightRatioBottom };
}

void ImGuiWindows::Render()
{
	StartWindow(0);
	SceneManager::GetInstance()->DrawInterface();
	EndWindow();

	StartWindow(1);
	GameInfo::GetInstance()->DrawInterface();
	EndWindow();

	StartWindow(2);
	GameObjectManager::GetInstance()->DrawInterface1();
	EndWindow();

	StartWindow(3);
	GameObjectManager::GetInstance()->DrawInterface2();
	EndWindow();
}

void ImGuiWindows::StartWindow(int i)
{
	SetNextWindowBgAlpha(1.f);
	SetNextWindowPos({ m_WindowPositions[i].x, m_WindowPositions[i].y }, ImGuiCond_Always);
	SetNextWindowSize({ m_WindowSizes[i].x, m_WindowSizes[i].y });
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	Begin(std::to_string(i).c_str());
	BeginTabBar(std::to_string(i).c_str(), tab_bar_flags);
}

void ImGuiWindows::EndWindow()
{
	EndTabBar();
	End();
}

