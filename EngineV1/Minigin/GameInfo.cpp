#include "MiniginPCH.h"
#include "GameInfo.h"


void GameInfo::Start()
{
	m_CurrFrame = std::chrono::high_resolution_clock::now();
	m_PrevFrame = m_CurrFrame;
}

void GameInfo::Update()
{
	m_CurrFrame = std::chrono::high_resolution_clock::now();
	m_ElapsedTime = std::chrono::duration<float>(m_CurrFrame - m_PrevFrame).count();
	m_PrevFrame = m_CurrFrame;
}

const Vector2f& GameInfo::GetWindowSize() const
{
	return m_WindowSize;
}

void GameInfo::SetWindowSize(const Vector2f& windowSize)
{
	m_WindowSize = windowSize;
}

float GameInfo::GetElapsedSeconds()
{
	return m_ElapsedTime;
}
