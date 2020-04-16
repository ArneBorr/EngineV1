#pragma once

#include "GameObject.h"

class BaseComponent
{
public:
	BaseComponent(std::weak_ptr<GameObject> pGameObject);
	virtual ~BaseComponent() = default;

	virtual void Render() = 0;
	virtual void Update(float elapsedSec) = 0;

protected:
	std::weak_ptr<GameObject> m_pGameObject;
};

