#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "GameObject.h"
#include "SaveHandler.h"
#include "Renderer.h"

void SceneManager::Initialize(SaveHandler* pSaveHandler)
{
	pSaveHandler->LoadScenes(m_pScenes);
	m_pCurrentScene = m_pScenes[0];
}

SceneManager::~SceneManager()
{
	for (unsigned int i{}; i < m_pScenes.size(); i++)
	{
		delete m_pScenes[i];
		m_pScenes[i] = nullptr;
	}
}

void SceneManager::DrawInterface()
{
	using namespace ImGui;

	if (BeginTabItem("Scenes"))
	{
		for (auto sceneIt = m_pScenes.begin(); sceneIt != m_pScenes.end();)
		{
			PushID(*sceneIt);
			if (Selectable((*sceneIt)->GetName().c_str(), m_pCurrentScene == (*sceneIt), 0, { 150,12 }))
			{
				m_pCurrentScene = (*sceneIt);
				GameObjectManager::GetInstance()->SetSelectedGameObject(nullptr);
			}

			SameLine();
			if (Button("YEET"))
			{
				delete (*sceneIt);
				(*sceneIt) = nullptr;
				sceneIt = m_pScenes.erase(sceneIt);
			}
			else
				++sceneIt;

			Separator();
			PopID();
		}

		static char sceneName[25]{"NewScene"};
		InputText("Name", sceneName, IM_ARRAYSIZE(sceneName));
		if (Button("AddScene"))
		{
			m_pScenes.push_back(new Scene(sceneName));
		}

		EndTabItem();
	}

	if (BeginTabItem("CurrentScene"))
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = AcceptDragDropPayload("GameObject"))
			{
				auto pGameObject = GameObjectManager::GetInstance()->GetSelectedGameObject();
				pGameObject->DetachThis();
				m_pCurrentScene->AddChild(pGameObject);
			}

			EndDragDropTarget();
		}

		m_pCurrentScene->DrawInterfaceObjects();
		EndTabItem();
	}
}

void SceneManager::AddScene(Scene* scene)
{
	m_pScenes.push_back(scene);
	m_pCurrentScene = scene;
}


void SceneManager::Update(float elapsedSec)
{
	if (m_pCurrentScene)
		m_pCurrentScene->Update(elapsedSec);
}

void SceneManager::Render()
{
	if (m_pCurrentScene)
		m_pCurrentScene->Render();
}

Vector2f SceneManager::ChangeToFullscreenCoord(const Vector2f& pos)
{
	//https://stackoverflow.com/questions/5294955/how-to-scale-down-a-range-of-numbers-with-a-known-min-and-max-value
	const Vector2f windowSize = GameInfo::GetInstance()->GetWindowSize();
	const Vector4f editorDimensions = Renderer::GetInstance()->GetEditorDimensions();
	const float editorXPlusWidth = editorDimensions.x + editorDimensions.z;
	const float editorYPlusHeight = editorDimensions.y + editorDimensions.w;

	const float x = (windowSize.x - 0) * (pos.x - editorDimensions.x) / (editorXPlusWidth - editorDimensions.x);
	const float y = (windowSize.y - 0) * (pos.y - editorDimensions.y) / (editorYPlusHeight - editorDimensions.y);

	return Vector2f(x, y);
}

Vector2f SceneManager::AdapatPositionToEditor(const Vector2f& pos)
{
	//https://stackoverflow.com/questions/5294955/how-to-scale-down-a-range-of-numbers-with-a-known-min-and-max-value
	const Vector2f windowSize = GameInfo::GetInstance()->GetWindowSize();
	const Vector4f editorDimensions = Renderer::GetInstance()->GetEditorDimensions();
	const float editorXPlusWidth = editorDimensions.x + editorDimensions.z;
	const float editorYPlusHeight = editorDimensions.y + editorDimensions.w;

	const float x = (editorXPlusWidth - editorDimensions.x) * (pos.x - 0) / (windowSize.x - 0) + editorDimensions.x;
	const float y = (editorYPlusHeight - editorDimensions.y) * (pos.y - 0) / (windowSize.y - 0) + editorDimensions.y;
	
	return  Vector2f{ x, y };
}

Vector2f SceneManager::AdaptScaleToFullscreen(const Vector2f& scale)
{
	const Vector2f windowSize = GameInfo::GetInstance()->GetWindowSize();
	const Vector4f editorDimensions = Renderer::GetInstance()->GetEditorDimensions();

	const float scaleRatioX = windowSize.x / editorDimensions.z;
	const float scaleRatioY = windowSize.y / editorDimensions.w;

	return Vector2f{ scale.x * scaleRatioX, scale.y * scaleRatioY };
}

void SceneManager::StartPlaying()
{
	m_pCurrentScene->Initialize();
}

void SceneManager::StopPlaying()
{
	m_pCurrentScene->ResetObjects();
}

void SceneManager::AdaptToFullscreen(const Vector2f& ratio)
{
	for (auto scene : m_pScenes)
		scene->AdaptToFullScreen(ratio);
}

void SceneManager::SaveScenes(SaveHandler* pSaveHandler)
{
	pSaveHandler->SaveScenes(m_pScenes);
}

void SceneManager::SetScene(const std::string& name)
{
	auto it = std::find_if(m_pScenes.begin(), m_pScenes.end(), [name](Scene* pScene) { return name == pScene->GetName(); });
	if (it != m_pScenes.end())
	{
		m_pCurrentScene = (*it);
		m_pCurrentScene->Initialize();
	}
}
