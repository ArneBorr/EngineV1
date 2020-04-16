#pragma once

#include "Singleton.h"
#include <chrono>

class GameInfo final : public Singleton<GameInfo>
{
public:
	void Start();
	void Update();
	float GetElapsedSeconds();

private:
	std::chrono::time_point<std::chrono::steady_clock> m_CurrFrame;
	std::chrono::time_point<std::chrono::steady_clock> m_PrevFrame;
	float m_ElapsedTime;
};

