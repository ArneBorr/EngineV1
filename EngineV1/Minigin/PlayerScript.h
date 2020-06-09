#pragma once
#include "Script.h"

class AnimatorControllerComponent;
class RigidbodyComponent;

class PlayerScript final : public Script
{
public:
	PlayerScript();

	void Initialize() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

private:
	AnimatorControllerComponent* m_pAnimator{ nullptr };
	RigidbodyComponent* m_pRigidbody{ nullptr };
};

