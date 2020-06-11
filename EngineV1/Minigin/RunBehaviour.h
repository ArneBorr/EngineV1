#pragma once
#include "Behaviour.h"
class RunBehaviour final : public Behaviour
{
public:
	RunBehaviour();

	void Enter() override;
	Behaviour* HandleInput() override;
	void Update(float elapsesSec) override;
	void Render() override;
	void Exit() override;

	void DrawInterface() override;
	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(rapidxml::xml_node<>* node) override;

private:
	
};

