#pragma once
#include "SceneManager.h"


class GameObject;
class Scene
{
public:

	Scene();

	virtual ~Scene();

	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

	void Add(const std::shared_ptr<GameObject>& object);
	virtual void Initialize() = 0;
	virtual void Update(float elapsedSec);
	virtual void Render() const;

private: 
	explicit Scene(const std::string& name);

	std::string m_Name;
	std::vector < std::shared_ptr<GameObject>> m_Objects{};

	static unsigned int m_IdCounter; 
};

