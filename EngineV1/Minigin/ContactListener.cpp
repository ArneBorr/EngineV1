#include "MiniginPCH.h"
#include "ContactListener.h"
#include "BoxColliderComponent.h"
#include "RigidbodyComponent.h"

void ContactListener::BeginContact(b2Contact* pContact)
{
	const b2Fixture* pFictureA = pContact->GetFixtureA();
	const b2Fixture* pFictureB = pContact->GetFixtureB();

	BoxColliderComponent* pColliderA = static_cast<BoxColliderComponent*>(pFictureA->GetUserData());
	BoxColliderComponent* pColliderB = static_cast<BoxColliderComponent*>(pFictureB->GetUserData());

	if (!pColliderA || !pColliderB)
	{
		std::cout << "NoBoxColl\n";
		return;
	}

	if (pFictureA->IsSensor())
	{
		if (pColliderA->GetName() == "GroundDetector" && pColliderB->GetGameObject()->HasTags({ "Ground" }))
		{
			static_cast<RigidbodyComponent*>(pFictureA->GetBody()->GetUserData())->SetOnGround(true);
			std::cout << "OnGround\n";
		}

		pColliderA->RegisterCollision(pColliderB->GetGameObject()->GetTags(), true);
	}
	if (pFictureB->IsSensor())
	{
		if (pColliderB->GetName() == "GroundDetector" && pColliderA->GetGameObject()->HasTags({ "Ground" }))
		{
			static_cast<RigidbodyComponent*>(pFictureB->GetBody()->GetUserData())->SetOnGround(true);
			std::cout << "OnGround\n";
		}

		pColliderB->RegisterCollision(pColliderA->GetGameObject()->GetTags(), true);
	}
}

void ContactListener::EndContact(b2Contact* pContact)
{
	const b2Fixture* pFictureA = pContact->GetFixtureA();
	const b2Fixture* pFictureB = pContact->GetFixtureB();

	BoxColliderComponent* pColliderA = static_cast<BoxColliderComponent*>(pFictureA->GetUserData());
	BoxColliderComponent* pColliderB = static_cast<BoxColliderComponent*>(pFictureB->GetUserData());

	if (pFictureA->IsSensor())
	{
		if (pColliderA->GetName() == "GroundDetector" && pColliderB->GetName() == "Ground")
		{
			static_cast<RigidbodyComponent*>(pFictureA->GetBody()->GetUserData())->SetOnGround(false);
			std::cout << "OffGround\n";
		}

		pColliderA->RegisterCollision(pColliderB->GetGameObject()->GetTags(), false);
	}
	if (pFictureB->IsSensor())
	{
		if (pColliderB->GetName() == "GroundDetector" && pColliderA->GetName() == "Ground")
		{
			static_cast<RigidbodyComponent*>(pFictureB->GetBody()->GetUserData())->SetOnGround(false);
			std::cout << "OffGround\n";
		}

		pColliderB->RegisterCollision(pColliderA->GetGameObject()->GetTags(), false);
	}
}
