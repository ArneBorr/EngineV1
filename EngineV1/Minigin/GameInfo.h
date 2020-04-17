#pragma once

#include "Singleton.h"
#include <chrono>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

#include "structs.h"


class GameInfo final : public Singleton<GameInfo>
{
public:
	void Start();
	void Update();

	const Vector2f& GetWindowSize() const;
	void SetWindowSize(const Vector2f& windowSize);
	float GetElapsedSeconds();


private:
	Vector2f m_WindowSize{};

	std::chrono::time_point<std::chrono::steady_clock> m_CurrFrame;
	std::chrono::time_point<std::chrono::steady_clock> m_PrevFrame;
	float m_ElapsedTime{};
};

