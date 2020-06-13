#pragma once
#include "Box2D.h"

class RayCastCallback : public b2RayCastCallback
{
public:
	RayCastCallback() {};

	b2Fixture* GetClosestFixture() const { return m_pClosestFicure; }

private:
	b2Fixture* m_pClosestFicure{ nullptr };
	float m_ClosestFraction{ FLT_MAX };

	float ReportFixture(b2Fixture* pFixture, const b2Vec2& point, const b2Vec2& normal, float fraction);
};

