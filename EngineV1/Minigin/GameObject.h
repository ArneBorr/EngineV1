#pragma once
#include "SceneObject.h"
#include "TransformComponent.h"



class BaseComponent;
class GameObject final : public SceneObject
{
public:
	void Update(float elapsedSec) override;
	void Render() const override;

	void AddComponent(BaseComponent* pComponent);
	template <class T>
	T* GetComponent();
	TransformComponent GetTransform();

	GameObject() = default;
	virtual ~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

private:
	std::vector<BaseComponent*> m_pBaseComponents;
	TransformComponent m_Transform;
};

template<class T>
inline T* GameObject::GetComponent()
{
	const type_info& ti = typeid(T);
	for (auto pComp : m_pBaseComponents)
	{
		if (pComp && typeid(*pComp) == ti)
			return static_cast<T*>(pComp);
	}

	return nullptr;
}
