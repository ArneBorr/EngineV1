#pragma once

#include "GameObject.h"

class BaseComponent
{
public:
	BaseComponent(GameObject* pGameObject);
	virtual ~BaseComponent() = default;

	virtual void Render() = 0;
	virtual void Update(float elapsedSec) = 0;

protected:
	GameObject* m_pGameObject;
};

