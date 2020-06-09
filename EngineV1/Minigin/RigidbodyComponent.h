#pragma once
#include "BaseComponent.h"

class b2Body;
class BoxColliderComponent;

class RigidbodyComponent : public BaseComponent
{
public:
	RigidbodyComponent(GameObject* pObject);
	~RigidbodyComponent();

	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(const std::string& type, float density, float friction, float restitution);
	void ChangeShape(BoxColliderComponent* pBox, const b2PolygonShape& shape);

	void SetPosition(const Vector2f& pos);
	Vector2f GetPosition() const;
	void SetRotation(float rotation);
	float GetRotation() const { return m_pBody->GetAngle() * 180 / M_PI; }; // In Degrees
	Vector2f GetVelocity();
	void UpdateShapeScale();

private:
	friend class MovementComponent;

	void Move(const Vector2f& vel, const Vector2f& maxVel);
	void Jump(float strength);

	b2Body* m_pBody{ nullptr };
	b2Fixture* m_pFicture{ nullptr };
	float m_Density{ 10.f };
	float m_Friction{ 0.65f };
	float m_Restitution{ 0.3f };
	int m_TypeButtonIndex{ 0 };
};

