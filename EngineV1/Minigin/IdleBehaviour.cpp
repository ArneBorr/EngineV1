#include "MiniginPCH.h"
#include "IdleBehaviour.h"
#include "imgui.h"
#include "Sprite.h"

IdleBehaviour::IdleBehaviour()
	: Behaviour( "IdleBehaviour" )
{
}

void IdleBehaviour::Enter()
{
}

Behaviour* IdleBehaviour::HandleInput()
{
	return nullptr;
}

void IdleBehaviour::Update(float elapsesSec)
{
	UNREFERENCED_PARAMETER(elapsesSec);
}

void IdleBehaviour::Exit()
{
}

void IdleBehaviour::DrawInterface()
{
	using namespace ImGui;
	Separator();
	Text("Transitions");
	Separator();

	//Set transition, A lot of repetition because of the way imgui works
	Behaviour* temp;
	if (Button("RunTransition"))
		m_pRunTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pRunTransition = temp;
	PrintTransitionSet(m_pRunTransition);

	if (Button("JumpTransition"))
		m_pJumpTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pJumpTransition = temp;
	PrintTransitionSet(m_pJumpTransition);

	if (Button("ShootTransition"))
		m_pShootTransition = nullptr;
	temp = HandleTransitionDrop(this);
	if (temp)
		m_pShootTransition = temp;
	PrintTransitionSet(m_pShootTransition);

	Separator();
	Text("Sprite");
	Separator();
	if (m_pSprite)
		Button(m_pSprite->GetName().c_str());
	else
		Button("None");
	auto sprite = HandleSpriteDrop();
	if (sprite)
		m_pSprite = sprite;
}
