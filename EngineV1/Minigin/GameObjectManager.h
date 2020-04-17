#pragma once
class GameObject;

class GameObjectManager final
{
public:
	GameObjectManager() = default;
	~GameObjectManager() = default;

	void DrawInterface()  const;

private:
	void CreateEmptyGameObject() const;
};

