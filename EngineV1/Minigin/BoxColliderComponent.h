#pragma once
#include "BaseComponent.h"

class RigidbodyComponent;
class BoxColliderComponent : public BaseComponent
{
public:
	BoxColliderComponent(GameObject* pObject, RigidbodyComponent* pBody);
	~BoxColliderComponent();

	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node) override;

	void CreateLink(RigidbodyComponent* pBody);

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	float m_Width{ 200.f };
	float m_Height{ 200.f };

	void CreateShape();
};

