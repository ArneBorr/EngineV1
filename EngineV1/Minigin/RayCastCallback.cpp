#include "MiniginPCH.h"
#include "RayCastCallback.h"

float RayCastCallback::ReportFixture(b2Fixture* pFixture, const b2Vec2&, const b2Vec2&, float fraction)
{
	if (fraction < m_ClosestFraction)
	{
		m_ClosestFraction = fraction;
		m_pClosestFicure = pFixture;
	}

	return fraction;
}