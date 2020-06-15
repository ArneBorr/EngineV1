#pragma once
#include "box2d.h"

class ContactListener : public b2ContactListener
{
private:
	void BeginContact(b2Contact* pContact);
	void EndContact(b2Contact* pContact);
};

