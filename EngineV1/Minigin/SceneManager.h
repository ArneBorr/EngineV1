#pragma once
#include "Singleton.h"

class Scene;
class SaveHandler;

class SceneManager final : public Singleton<SceneManager>
{
public:
	~SceneManager();

	void DrawInterface();
	void AddScene(Scene* scene);

	void Initialize(const Vector2f& windowDimensions);
	void Update(float elapsedSec);
	void Render();

	Vector2f ChangeToFullscreenCoord(const Vector2f& pos);
	Vector2f AdapatPositionToView(const Vector2f& pos);
	Vector2f AdapatPositionToFullScreenw(const Vector2f& pos);
	Vector2f AdaptScaleToFullscreen(const Vector2f& scale);
	static const Vector4f& GetEditorWindowDimensions()  { return m_EditorDimensions; }

	void SaveScenes();

	Scene* GetCurrentScene() { return m_pCurrentScene; }; // Add Log if nullptr

private:
	SaveHandler* m_pSaveHandler;
	std::vector<Scene*> m_pScenes{};
	Scene* m_pCurrentScene{};

	static const Vector4f m_EditorDimensions; //x,y = left bottom | z,w = right top
	Vector2f m_WindowDimensions{};
	
};

