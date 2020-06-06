#pragma once
#include "BaseComponent.h"

class b2Body;
class RigidbodyComponent : public BaseComponent
{
public:
	RigidbodyComponent(GameObject* pObject, const std::string& name);
	~RigidbodyComponent();

	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node) override;
private:
	b2Body* m_pBody;
};

