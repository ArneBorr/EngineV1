#pragma once
#include "Script.h"

class RigidbodyComponent;

class AllowOneWay : public Script
{
public:
	AllowOneWay();

	void Initialize() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	bool m_IsCollisionIgnored{ false };
};

