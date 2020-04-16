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
	std::weak_ptr<GameObject> m_FpsCounter;

	float m_FPSInterval;
};
