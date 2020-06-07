#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "GameObject.h"
#include "SaveHandler.h"

const Vector4f SceneManager::m_EditorDimensions = Vector4f{ 255, 100, 1026, 536 }; //848x480

void SceneManager::Initialize(const Vector2f& windowDimensions)
{
	m_WindowDimensions = windowDimensions;

	m_pSaveHandler->Load(m_pScenes);

	m_pCurrentScene = m_pScenes[0];
}

SceneManager::~SceneManager()
{
	for (unsigned int i{}; i < m_pScenes.size(); i++)
	{
		delete m_pScenes[i];
		m_pScenes[i] = nullptr;
	}

	delete m_pSaveHandler;
	m_pSaveHandler = nullptr;
}

void SceneManager::DrawInterface()
{

	//ImGui::SetNextWindowPos({ 0,0 }, ImGuiCond_Always);
	//auto windowSize = GameInfo::GetInstance()->GetWindowSize();
	//static float widthManagerRatio = 0.2f;
	//static float heightManagerRatio = 1.f;
	//ImGui::SetNextWindowSize({ windowSize.x * widthManagerRatio, windowSize.y * heightManagerRatio });
	//ImGui::SetNextWindowBgAlpha(1.f);
	//ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabItem("Scenes"))
	{
		ImGui::EndTabItem();
		ImGui::Button("AddScene");
	}

	if (ImGui::BeginTabItem("CurrentScene"))
	{
		ImGui::EndTabItem();
			
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
			{
				auto pObject = new GameObject(std::move(*(GameObject*)(payload->Data)));
				m_pCurrentScene->AddChild(pObject);
			}

			ImGui::EndDragDropTarget();
		}

		m_pCurrentScene->DrawInterfaceObjects();
	}
}

void SceneManager::AddScene(Scene* scene)
{
	m_pScenes.push_back(scene);
	m_pCurrentScene = scene;
}


void SceneManager::Update(float elapsedSec)
{
	for (auto& scene : m_pScenes)
	{
		scene->Update(elapsedSec);
	}
}

void SceneManager::Render()
{
	for (const auto& scene : m_pScenes)
	{
		scene->Render();
	}
}

Vector2f SceneManager::ChangeToFullscreenCoord(const Vector2f& pos)
{
	float x{}, y{};
	if (!GameInfo::GetInstance()->IsFullscreen())
	{
		x = (m_WindowDimensions.x - 0) * (pos.x - m_EditorDimensions.x) / (m_EditorDimensions.z - m_EditorDimensions.x);
		y = (m_WindowDimensions.y - 0) * (pos.y - m_EditorDimensions.y) / (m_EditorDimensions.w - m_EditorDimensions.y);
		return Vector2f(x, y);
	}

	return pos;
}

Vector2f SceneManager::AdapatPositionToView(const Vector2f& pos)
{
	//https://stackoverflow.com/questions/5294955/how-to-scale-down-a-range-of-numbers-with-a-known-min-and-max-value
	float x{}, y{};
	if (GameInfo::GetInstance()->IsFullscreen())
	{
		AdapatPositionToFullScreenw(pos);
	}
	else
	{	
		x = (m_EditorDimensions.z - m_EditorDimensions.x) * (pos.x - 0) / (m_WindowDimensions.x - 0) + m_EditorDimensions.x;
		y = (m_EditorDimensions.w - m_EditorDimensions.y) * (pos.y - 0) / (m_WindowDimensions.y - 0) + m_EditorDimensions.y;
	}
	return  Vector2f{ x, y };
}

Vector2f SceneManager::AdapatPositionToFullScreenw(const Vector2f& pos)
{
	float x = (m_WindowDimensions.x - 0) * (pos.x - m_EditorDimensions.x) / (m_EditorDimensions.z - m_EditorDimensions.x);
	float y = (m_WindowDimensions.y - 0) * (pos.y - m_EditorDimensions.y) / (m_EditorDimensions.w - m_EditorDimensions.y);
	return Vector2f{ x, y };
}

Vector2f SceneManager::AdaptScaleToFullscreen(const Vector2f& scale)
{
	const float scaleRatioX = m_WindowDimensions.x / (m_EditorDimensions.z - m_EditorDimensions.x);
	const float scaleRatioY = m_WindowDimensions.y / (m_EditorDimensions.w - m_EditorDimensions.y);

	return Vector2f{ scale.x * scaleRatioX, scale.y * scaleRatioY };
}

void SceneManager::SaveScenes()
{
	m_pSaveHandler->Save(m_pScenes);
}
