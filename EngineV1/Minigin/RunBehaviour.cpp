#include "MiniginPCH.h"
#include "RunBehaviour.h"

RunBehaviour::RunBehaviour()
	: Behaviour("RunBehaviour")
{
}

void RunBehaviour::Enter()
{
}

Behaviour* RunBehaviour::HandleInput()
{
	return nullptr;
}

void RunBehaviour::Update(float elapsesSec)
{
	UNREFERENCED_PARAMETER(elapsesSec);
}

void RunBehaviour::Exit()
{
}

void RunBehaviour::DrawInterface()
{
}
