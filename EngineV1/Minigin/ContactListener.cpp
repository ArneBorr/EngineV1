#include "MiniginPCH.h"
#include "ContactListener.h"
#include "BoxColliderComponent.h"
#include "RigidbodyComponent.h"

void ContactListener::BeginContact(b2Contact* pContact)
{
	const b2Fixture* pFictureA = pContact->GetFixtureA();
	const b2Fixture* pFictureB = pContact->GetFixtureB();

	auto userDataA = pFictureA->GetUserData();
	auto userDataB = pFictureB->GetUserData();

	BoxColliderComponent* pColliderA = static_cast<BoxColliderComponent*>(userDataA);
	BoxColliderComponent* pColliderB = static_cast<BoxColliderComponent*>(userDataB);

	if (!pColliderA || !pColliderB || (pColliderA->GetGameObject() == pColliderB->GetGameObject()))
		return;

	if (pFictureA->IsSensor())
	{
		if (pColliderA->GetName() == "GroundDetector" && pColliderB->GetGameObject()->HasTags({ "Ground" }))
		{
			auto rigidbody = static_cast<RigidbodyComponent*>(pFictureA->GetBody()->GetUserData());
			if (rigidbody)
				rigidbody->SetOnGround(true);
		}
		else if (pColliderA->GetName() != "GroundDetector") // Ground detector does not trigger events
			pColliderA->RegisterCollision(pColliderA->GetGameObject(), pColliderB->GetGameObject(), true);
	}
	if (pFictureB->IsSensor())
	{
		if (pColliderB->GetName() == "GroundDetector" && pColliderA->GetGameObject()->HasTags({ "Ground" }))
		{
			auto rigidbody = static_cast<RigidbodyComponent*>(pFictureB->GetBody()->GetUserData());
			if (rigidbody)
				rigidbody->SetOnGround(true);
		}
		else if (pColliderB->GetName() != "GroundDetector") // Ground detector does not trigger events
			pColliderB->RegisterCollision(pColliderB->GetGameObject(), pColliderA->GetGameObject(), true);
	}
}

void ContactListener::EndContact(b2Contact* pContact)
{
	const b2Fixture* pFictureA = pContact->GetFixtureA();
	const b2Fixture* pFictureB = pContact->GetFixtureB();

	auto userDataA = pFictureA->GetUserData();
	auto userDataB = pFictureB->GetUserData();

	BoxColliderComponent* pColliderA = static_cast<BoxColliderComponent*>(userDataA);
	BoxColliderComponent* pColliderB = static_cast<BoxColliderComponent*>(userDataB);

	if (!pColliderA || !pColliderB || (pColliderA->GetGameObject() == pColliderB->GetGameObject())) // No self collision
		return;

	if (pFictureA->IsSensor())
	{
		if (pColliderA->GetName() == "GroundDetector" && pColliderA->GetGameObject()->HasTags({ "Ground" }))
		{
			auto rigidbody = static_cast<RigidbodyComponent*>(pFictureA->GetBody()->GetUserData());
			if (rigidbody)
				rigidbody->SetOnGround(false);
		}
		else if (pColliderA->GetName() != "GroundDetector") // Ground detector does not trigger events
			pColliderA->RegisterCollision(pColliderA->GetGameObject(), pColliderB->GetGameObject(), false);
	}
	if (pFictureB->IsSensor())
	{
		if (pColliderB->GetName() == "GroundDetector" && pColliderA->GetGameObject()->HasTags({"Ground"}))
		{
			auto rigidbody = static_cast<RigidbodyComponent*>(pFictureB->GetBody()->GetUserData());
			if (rigidbody)
				rigidbody->SetOnGround(false);
		}
		else if (pColliderB->GetName() != "GroundDetector") // Ground detector does not trigger events
			pColliderB->RegisterCollision(pColliderB->GetGameObject(), pColliderA->GetGameObject(), false);
	}
}
