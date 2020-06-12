#include "MiniginPCH.h"
#include "ContactListener.h"
#include "BoxColliderComponent.h"

void ContactListener::BeginContact(b2Contact* pContact)
{
	const b2Fixture* pFictureA = pContact->GetFixtureA();
	const b2Fixture* pFictureB = pContact->GetFixtureB();

	BoxColliderComponent* pColliderA = static_cast<BoxColliderComponent*>(pFictureA->GetUserData());
	BoxColliderComponent* pColliderB = static_cast<BoxColliderComponent*>(pFictureB->GetUserData());

	if (pFictureA->IsSensor())
		pColliderA->RegisterCollision(pColliderB->GetGameObject()->GetTags(), true);
	if (pFictureB->IsSensor())
		pColliderB->RegisterCollision(pColliderA->GetGameObject()->GetTags(), true);
}

void ContactListener::EndContact(b2Contact* pContact)
{
	const b2Fixture* pFictureA = pContact->GetFixtureA();
	const b2Fixture* pFictureB = pContact->GetFixtureB();

	BoxColliderComponent* pColliderA = static_cast<BoxColliderComponent*>(pFictureA->GetUserData());
	BoxColliderComponent* pColliderB = static_cast<BoxColliderComponent*>(pFictureB->GetUserData());

	if (pFictureA->IsSensor())
		pColliderA->RegisterCollision(pColliderB->GetGameObject()->GetTags(), false);
	if (pFictureB->IsSensor())
		pColliderB->RegisterCollision(pColliderA->GetGameObject()->GetTags(), false);
}
