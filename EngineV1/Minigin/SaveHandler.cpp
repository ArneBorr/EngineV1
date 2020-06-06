#include "MiniginPCH.h"
#include "SaveHandler.h"
#include "rapidxml_print.hpp"
#include "Scene.h"
#include "GameObject.h"
#include <fstream>
#include "Components.h"
#include "Font.h"

const std::string SaveHandler::m_FilePath{ "../Minigin/Save.xml" };

void SaveHandler::Save(const std::vector<Scene*>& scenes)
{
	using namespace rapidxml;
	xml_document<> doc;

	xml_node<>* root = doc.allocate_node(node_element, "Root");
	root->append_attribute(doc.allocate_attribute("Game", "BubbleBobble"));
	doc.append_node(root);

	//Scenes
	//***********
	for (Scene* scene : scenes)
	{
		xml_node<>* sceneNode = doc.allocate_node(node_element, "Scene");
		sceneNode->append_attribute(doc.allocate_attribute("Name", scene->GetName().c_str()));
		root->append_node(sceneNode);

		//Objects
		//***********
		for (GameObject* object : scene->GetGameObjects())
		{
			object->SaveAttributes(doc, sceneNode);
		}
	}

	std::ofstream file_stored(m_FilePath);
	file_stored << doc;
	file_stored.close();
}

void SaveHandler::Load(std::vector<Scene*>& scenes)
{
	UNREFERENCED_PARAMETER(scenes);
	using namespace rapidxml;

	xml_document<> doc;
	std::ifstream file(m_FilePath);

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);

	auto rootNode = doc.first_node();

	//Scenes
	//***********
	for (auto sceneNode = rootNode->first_node("Scene"); sceneNode; sceneNode = sceneNode->next_sibling())
	{
		Scene* scene = new Scene(sceneNode->first_attribute("Name")->value());
		scenes.push_back(scene);

		//Objects
		//***********
		for (auto objectNode = sceneNode->first_node("GameObject"); objectNode; objectNode = objectNode->next_sibling())
			scene->AddChild(LoadObject(objectNode));
	}
}

GameObject* SaveHandler::LoadObject(rapidxml::xml_node<>* node)
{
	using namespace rapidxml;
	GameObject* object = new GameObject(node->first_attribute("Name")->value());

	xml_node<>* componentsNode = node->first_node("Components");

	if (componentsNode != 0)
	{
		//Components
		//***********
		for (xml_node<>* componentNode = componentsNode->first_node(); componentNode != 0; componentNode =componentNode->next_sibling())
		{
			
			BaseComponent* component = nullptr;
			if (strcmp(componentNode->name(), "TransformComponent") == 0)
			{
				component = LoadTransformComponent(componentNode, object);
			}
			else if (strcmp(componentNode->name(), "TextureComponent") == 0)
			{
				component = LoadTextureComponent(componentNode, object);
			}
			else if (strcmp(componentNode->name(), "TextComponent") == 0)
			{
				component = LoadTextComponent(componentNode, object);
			}

			if (!component)
			{
				std::printf("Component not found\n"); //LOGGER
				continue;
			}

			object->AddComponent(component);
		}
	}

	//Children
	//***********
	for (auto childNode = node->first_node("GameObject"); childNode; childNode = childNode->next_sibling())
		object->AddChild(LoadObject(childNode));
	
	
	return object;
}

TransformComponent* SaveHandler::LoadTransformComponent(rapidxml::xml_node<>* node, GameObject* object)
{

	const float posX = std::stof(node->first_attribute("PosX")->value());
	const float posY = std::stof(node->first_attribute("PosY")->value());
	const float scaleX = std::stof(node->first_attribute("ScaleX")->value());
	const float scaleY = std::stof(node->first_attribute("ScaleY")->value());

	TransformComponent* component = new TransformComponent(object);
	component->SetPosition(posX, posY);
	component->SetScale(scaleX, scaleY);

	return component;
}

TextureComponent* SaveHandler::LoadTextureComponent(rapidxml::xml_node<>* node, GameObject* object)
{
	const std::string texturePath = node->first_attribute("TexturePath")->value();
	const float offsetX = std::stof(node->first_attribute("OffsetX")->value());
	const float offsetY = std::stof(node->first_attribute("OffsetY")->value());

	TextureComponent* component = new TextureComponent(object, texturePath);
	component->SetPosition(offsetX, offsetY);

	return component;
}

TextComponent* SaveHandler::LoadTextComponent(rapidxml::xml_node<>* node, GameObject* object)
{
	std::string fontPath = node->first_attribute("FontPath")->value();
	int fontSize = std::stoi(node->first_attribute("FontSize")->value());
	
	std::string text = node->first_attribute("Text")->value();
	const float offsetX = std::stof(node->first_attribute("OffsetX")->value());
	const float offsetY = std::stof(node->first_attribute("OffsetY")->value());

	TextComponent* component = new TextComponent(object,new Font(fontPath, fontSize), text);
	component->SetPosition(offsetX, offsetY);

	return component;
}
