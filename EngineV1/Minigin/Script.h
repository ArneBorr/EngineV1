#pragma once
#include "Observer.h"

class GameObject;

class Script : public Observer
{
public:
	Script(const std::string& name);
	virtual ~Script() = default;

	virtual void Initialize() {}
	virtual void Update(float) {}
	virtual void DrawInterface() {}
	virtual void OnNotify(const std::string&, GameObject*, GameObject*) {}
	virtual void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node);
	virtual void SetAttributes(rapidxml::xml_node<>* ) {  };

	void SetGameObject(GameObject* pObject) { m_pGameObject = pObject; }
	const std::string& GetName() { return m_Name; }

protected:
	GameObject* m_pGameObject{ nullptr };
	std::string m_Name;
};

