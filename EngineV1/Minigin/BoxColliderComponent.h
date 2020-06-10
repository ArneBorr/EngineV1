#pragma once
#include "BaseComponent.h"

class RigidbodyComponent;
class Texture2D;

class BoxColliderComponent : public BaseComponent
{
public:
	BoxColliderComponent(GameObject* pObject, RigidbodyComponent* pBody);
	~BoxColliderComponent();

	BoxColliderComponent(const BoxColliderComponent& other) noexcept;
	BoxColliderComponent(BoxColliderComponent&& other) = delete;
	BoxColliderComponent& operator=(const BoxColliderComponent& other) = delete;
	BoxColliderComponent& operator=(BoxColliderComponent&& other) = delete;

	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(float width, float height, int renderCollider);

	void CreateLink(RigidbodyComponent* pBody);

	void SetObject(GameObject* pObject) override;

private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	Texture2D* m_pTexture{};
	float m_Width{ 200.f };
	float m_Height{ 200.f };
	bool m_RenderCollider{ true };

	void CreateShape();
};

