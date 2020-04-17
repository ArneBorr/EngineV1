#pragma once

#include "GameObject.h"

class BaseComponent
{
public:
	BaseComponent(GameObject* pGameObject, const std::string& name);
	virtual ~BaseComponent() = default;

	virtual void Render() = 0;
	virtual void Update(float elapsedSec) = 0;

	virtual void DrawInterface() = 0;

	const std::string& GetName() const { return m_Name; }
	void SetName(const std::string& name) { m_Name = name; }

protected:
	GameObject* m_pGameObject;

private:
	std::string m_Name;
};

