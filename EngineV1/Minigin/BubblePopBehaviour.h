#pragma once
#include "Behaviour.h"

class BubblePopBehaviour : public Behaviour
{
public:
	BubblePopBehaviour();

	Behaviour* HandleInput() override;
	void DrawInterface() override;
	void Update(float elapsedSec) override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;

private:
};

