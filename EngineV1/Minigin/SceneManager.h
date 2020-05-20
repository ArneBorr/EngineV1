#pragma once
#include "Singleton.h"


class Scene;

class SceneManager final : public Singleton<SceneManager>
{
public:
	~SceneManager();

	void DrawInterface();
	void AddScene(Scene* scene);

	void Initialize(const Vector2f& windowDimensions);
	void Update(float elapsedSec);
	void Render();

	Vector2f AdaptLocationToEditor(const Vector2f& loc);
	Vector2f AdaptLocationToFullscreen(const Vector2f& loc);
	Vector2f AdaptScaleToFullscreen(const Vector2f& scale);
	static const Vector4f& GetEditorWindowDimensions()  { return m_EditorDimensions; }

	Scene* GetCurrentScene() { return m_pCurrentScene; }; // Add Log if nullptr

private:
	std::vector<Scene*> m_pScenes{};
	Scene* m_pCurrentScene{};

	static const Vector4f m_EditorDimensions; //x,y = left bottom | z,w = right top
	Vector2f m_WindowDimensions;
	
};

