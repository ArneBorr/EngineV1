#pragma once
#include "Singleton.h"

class Scene;
class SaveHandler;

class SceneManager final : public Singleton<SceneManager>
{
public:
	~SceneManager();

	void DrawInterface();
	void AddScene(Scene* pScene);

	void Initialize(SaveHandler* pSaveHandler);
	void Update(float elapsedSec);
	void Render();

	Vector2f ChangeToFullscreenCoord(const Vector2f& pos);
	Vector2f AdapatPositionToEditor(const Vector2f& pos);
	Vector2f AdaptScaleToFullscreen(const Vector2f& scale);

	void StartPlaying();
	void StopPlaying();

	void AdaptToFullscreen(const Vector2f& ratio);

	void SaveScenes(SaveHandler* pSaveHandler);

	Scene* GetCurrentScene() { return m_pCurrentScene; }
	void SetScene(const std::string& name);
	void SetForReset() { m_HasToReset = true; };

private:
	std::vector<Scene*> m_pScenes;
	Scene* m_pCurrentScene = nullptr;
	bool m_HasToReset = false;
	void Reset();
};

