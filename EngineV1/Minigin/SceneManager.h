#pragma once
#include "Singleton.h"


class Scene;
class SceneManager final : public Singleton<SceneManager>
{
public:
	~SceneManager();

	void DrawInterface();
	void AddScene(Scene* scene);

	void Initialize();
	void Update(float elapsedSec);
	void Render();

	Scene* GetCurrentScene() { return m_pCurrentScene; }; // Add Log if nullptr

private:
	std::vector<Scene*> m_pScenes{};
	Scene* m_pCurrentScene{};
};

