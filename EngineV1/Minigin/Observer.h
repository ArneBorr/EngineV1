#pragma once
class GameObject;

class Observer
{
public:
	virtual ~Observer() {};
	virtual void OnNotify(const std::string& event, GameObject* pObject) = 0;
};

