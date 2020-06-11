#pragma once
#include "Behaviour.h"
class RunBehaviour final : public Behaviour
{
public:
	RunBehaviour();

	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsesSec) override;
	void Exit() override;

	void DrawInterface() override;

private:
	
};

