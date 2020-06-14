#include "MiniginPCH.h"
#include "LinkToSceneOverhead.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "ScriptComponent.h"
#include "Subject.h"

LinkToSceneOverhead::LinkToSceneOverhead()
	: Script("LinkToSceneOverhead")
{
}

void LinkToSceneOverhead::Initialize()
{
	auto rigidBody = m_pGameObject->GetRigidbody();
	auto overhead = m_pGameObject->GetScene()->GetOverhead();
	if (overhead && rigidBody)
	{
		auto scriptComponents = overhead->GetComponents<ScriptComponent>();
		for (auto scriptComp : scriptComponents)
		{
			auto script = scriptComp->GetScript();
			if (script)
				rigidBody->GetSubject()->AddObserver(script);
		}
	}
}
