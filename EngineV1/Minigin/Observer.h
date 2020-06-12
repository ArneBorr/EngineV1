#pragma once
class Observer
{
public:
	virtual ~Observer() {};
	virtual void OnNotify(const std::string& event) = 0;
};

