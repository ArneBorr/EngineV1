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

float GameInfo::GetElapsedSeconds()
{
	return m_ElapsedTime;
}
