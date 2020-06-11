#include "MiniginPCH.h"
#include "ImGuiWindows.h"
#include "GameObjectManager.h"
#include "imgui.h"
#include "SceneManager.h"
#include "InputManager.h"


using namespace ImGui;

ImGuiWindows::ImGuiWindows()
{
	const Vector2f windowSize{ GameInfo::GetInstance()->GetWindowSize() };

	const float widthRatioVertical = 0.2f;
	const float heightRatioVertical = 1.f;
	const float widthRatioHorizontal = 0.6f;
	const float heightRatioTop = 0.14f;
	const float heightRatioBottom = 0.26f;

	m_WindowPositions[0] = { 0,0 };
	m_WindowSizes[0] = { windowSize.x * widthRatioVertical, windowSize.y * heightRatioVertical };

	m_WindowPositions[1] = { windowSize.x * widthRatioVertical, 0 };
	m_WindowSizes[1] = { windowSize.x * widthRatioHorizontal, windowSize.y * heightRatioTop };

	m_WindowPositions[2] = { windowSize.x - windowSize.x * widthRatioVertical, 0 };
	m_WindowSizes[2] = { windowSize.x * widthRatioVertical, windowSize.y * heightRatioVertical };


	m_WindowPositions[3] = { windowSize.x * widthRatioVertical, windowSize.y - windowSize.y * heightRatioBottom };
	m_WindowSizes[3] = { windowSize.x * widthRatioHorizontal, windowSize.y * heightRatioBottom };

	m_EditorDimensions.x = windowSize.x * widthRatioVertical;
	m_EditorDimensions.y = windowSize.y * heightRatioTop;
	m_EditorDimensions.z = windowSize.x * (1 - widthRatioVertical * 2);
	m_EditorDimensions.w = windowSize.y * (1 - heightRatioTop - heightRatioBottom);

	m_WindowPositions[4] = { m_EditorDimensions.x, m_EditorDimensions.y };
	m_WindowSizes[4] = { m_EditorDimensions.z, m_EditorDimensions.w };
}

void ImGuiWindows::Render()
{
	StartWindow(0);
	SceneManager::GetInstance()->DrawInterface();
	InputManager::GetInstance()->DrawInterface();
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

	EditorWindow();
	//ShowDemoWindow();
}

void ImGuiWindows::StartWindow(int i)
{
	SetNextWindowBgAlpha(1.f);
	SetNextWindowPos({ m_WindowPositions[i].x, m_WindowPositions[i].y }, ImGuiCond_Always);
	SetNextWindowSize({ m_WindowSizes[i].x, m_WindowSizes[i].y });
	Begin(std::to_string(i).c_str(), nullptr, ImGuiWindowFlags_NoTitleBar);
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	BeginTabBar(std::to_string(i).c_str(), tab_bar_flags);
}

void ImGuiWindows::EndWindow()
{
	EndTabBar();
	End();
}

void ImGuiWindows::EditorWindow()
{
	SetNextWindowBgAlpha(1.f);
	SetNextWindowPos({ m_WindowPositions[4].x,  m_WindowPositions[4].y }, ImGuiCond_Always);
	SetNextWindowSize({ m_WindowSizes[4].x, m_WindowSizes[4].y });

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	Begin("HeheBoy", nullptr, window_flags);

	
	End();
}

