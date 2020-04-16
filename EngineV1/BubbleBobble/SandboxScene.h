#pragma once

#include "Scene.h"

class GameObject;
class SandboxScene : public Scene
{
public:
	SandboxScene();

protected:
	void Initialize() override;
	void Update(float elapsedSec) override;

private:
	GameObject* m_pFpsCounter{ nullptr };

	float m_FPSInterval{};
};
