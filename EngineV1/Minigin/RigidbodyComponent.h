#pragma once
#include "BaseComponent.h"

class b2Body;
class BoxColliderComponent;
class Subject;

class RigidbodyComponent : public BaseComponent
{
public:
	RigidbodyComponent(GameObject* pObject);
	~RigidbodyComponent();

	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode) override;
	void SetAttributes(BoxColliderComponent* pGroundDetector, const std::string& type, bool fixedRot);

	b2Fixture* AddShape(const b2FixtureDef& fictureDef);
	void DestroyShape(b2Fixture* pFicture);
	void EraseCollider(BoxColliderComponent* pComponent);

	void SetPosition(const Vector2f& pos);
	Vector2f GetPosition() const;

	void SetRotation(float rotation);
	float GetRotation() const { return m_pBody->GetAngle() * 180 / M_PI; }; // In Degrees
	void SetFixedRotatoon(bool);
	void SetAngularVelocity(float vel);

	Vector2f GetVelocity() const;
	void SetLinearVelocity(const Vector2f& vel);
	void SetRestitution(float restitution);

	void SetIgnoreGroup(int group, bool ignore);
	void AddCollider(BoxColliderComponent* pCollider);

	void UpdateShapeScale();

	void MoveHorizontal(const Vector2f& vel);
	void MoveVertical(float y);
	void Jump(float strength);

	void SetOnGround(bool onGround) { m_IsOnGround = onGround; }
	bool IsOnGround() { return m_IsOnGround; }

	Subject* GetSubject() const { return m_pSubject; }

private:
	std::vector<BoxColliderComponent*> m_pColliders = {};
	BoxColliderComponent* m_pGroundDetection = nullptr;
	Subject* m_pSubject = nullptr;
	b2Body* m_pBody = nullptr;

	int m_TypeButtonIndex = 0;
	bool m_HasFixedRotation = false;
	bool m_DrawGroundDetector = false;
	bool m_IsOnGround = false;

	void CreateGroundDetector();
};

