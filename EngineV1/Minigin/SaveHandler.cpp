#include "MiniginPCH.h"
#include "SaveHandler.h"
#include "rapidxml_print.hpp"
#include "Scene.h"
#include "GameObject.h"
#include <fstream>
#include "Components.h"
#include "Font.h"
#include "InputManager.h"
#include "GameObjectManager.h"
#include "Script.h"
#include "PlayerScript.h"

const std::string SaveHandler::m_FilePathScenes{ "Data/SaveScenes.xml" };
const std::string SaveHandler::m_FilePathInput{ "Data/SaveInput.xml" };

void SaveHandler::SaveScenes(const std::vector<Scene*>& scenes)
{
	using namespace rapidxml;
	xml_document<>* doc{ new xml_document<>() };

	xml_node<>* root = doc->allocate_node(node_element, "Root");
	root->append_attribute(doc->allocate_attribute("Game", "BubbleBobble"));
	doc->append_node(root);

	//Scenes
	//***********
	for (Scene* scene : scenes)
	{
		xml_node<>* sceneNode = doc->allocate_node(node_element, "Scene");
		sceneNode->append_attribute(doc->allocate_attribute("Name", scene->GetName().c_str()));
		root->append_node(sceneNode);

		//Objects
		//***********
		for (GameObject* object : scene->GetGameObjects())
		{
			object->SaveAttributes(doc, sceneNode);
		}
	}

	std::ofstream file_stored(m_FilePathScenes);
	file_stored << doc;
	file_stored.close();

	delete doc;
	doc = nullptr;
}

void SaveHandler::LoadScenes(std::vector<Scene*>& scenes)
{
	using namespace rapidxml;

	std::cout << "Reached Initialize\n";
	xml_document<>* doc{ new xml_document<>() };
	std::ifstream file(m_FilePathScenes);

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content(buffer.str());
	doc->parse<0>(&content[0]);
	auto rootNode = doc->first_node();

	//Scenes
	//***********
	for (auto sceneNode = rootNode->first_node("Scene"); sceneNode; sceneNode = sceneNode->next_sibling())
	{
		Scene* scene = new Scene(sceneNode->first_attribute("Name")->value());
		scenes.push_back(scene);

		//Objects
		//***********
		for (auto objectNode = sceneNode->first_node("GameObject"); objectNode; objectNode = objectNode->next_sibling())
			scene->AddChild(LoadObject(objectNode, scene));
	}

	delete doc;
	doc = nullptr;
}

void SaveHandler::SaveInput(const std::map<std::string, std::vector<KeyboardButton*>>& actions)
{
	using namespace rapidxml;
	xml_document<>* doc{ new xml_document<>() };

	xml_node<>* root = doc->allocate_node(node_element, "Root");
	root->append_attribute(doc->allocate_attribute("Game", "BubbleBobble"));
	doc->append_node(root);

	//Keys
	//***********
	for (const auto& action : actions)
	{
		xml_node<>* actionNode = doc->allocate_node(node_element, "Action");
		actionNode->append_attribute(doc->allocate_attribute("Name", action.first.c_str()));
		auto oke = actionNode->first_attribute("Name")->value();
		UNREFERENCED_PARAMETER(oke);
		for (KeyboardButton* key : action.second)
		{
			xml_node<>* keyNode = doc->allocate_node(node_element, "Key");
			keyNode->append_attribute(doc->allocate_attribute("Name", key->name.c_str()));
			keyNode->append_attribute(doc->allocate_attribute("ID", IntToXMLChar(doc, key->id)));
			actionNode->append_node(keyNode);
		}
			
		root->append_node(actionNode);
	}

	std::ofstream file_stored(m_FilePathInput);
	file_stored << doc;
	file_stored.close();

	delete doc;
	doc = nullptr;
}

void SaveHandler::LoadInput(std::map<std::string, std::vector<KeyboardButton*>>& actions, const std::map<int, KeyboardButton*>& keys)
{
	using namespace rapidxml;

	xml_document<>* doc{ new xml_document<>() };
	std::ifstream file(m_FilePathInput);
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content(buffer.str());
	doc->parse<0>(&content[0]);

	auto rootNode = doc->first_node();

	//Actions
	//***********
	
	for (auto actionNode = rootNode->first_node("Action"); actionNode; actionNode = actionNode->next_sibling())
	{
		std::cout << "YES\n";
		std::pair<std::string, std::vector<KeyboardButton*>> action{ actionNode->first_attribute("Name")->value(), { } };
		//Keys
		//***********
		for (auto keyNode = actionNode->first_node("Key"); keyNode; keyNode = keyNode->next_sibling())
		{
			const int id = std::stoi(keyNode->first_attribute("ID")->value());
			action.second.push_back(keys.at(id));
		}

		actions.insert(action);
	}

	delete doc;
	doc = nullptr;
}

GameObject* SaveHandler::LoadObject(rapidxml::xml_node<>* node, Scene* scene)
{
	using namespace rapidxml;
	GameObject* object = new GameObject(node->first_attribute("Name")->value());
	object->SetScene(scene);
	xml_node<>* componentsNode = node->first_node("Components");

	if (componentsNode != 0)
	{
		//Components
		//***********
		for (xml_node<>* componentNode = componentsNode->first_node(); componentNode != 0; componentNode =componentNode->next_sibling())
		{
			//Make this cleaner
			BaseComponent* component = nullptr;
			if (strcmp(componentNode->name(), "TransformComponent") == 0)
			{
				object->SetTransform(LoadTransformComponent(componentNode, object));
				continue; // transform is not in general components list of object
			}
			else if (strcmp(componentNode->name(), "TextureComponent") == 0)
			{
				component = LoadTextureComponent(componentNode, object);
			}
			else if (strcmp(componentNode->name(), "TextComponent") == 0)
			{
				component = LoadTextComponent(componentNode, object);
			}
			else if (strcmp(componentNode->name(), "RigidbodyComponent") == 0)
			{
				component = LoadRigidbodyComponent(componentNode, object);
			}
			else if (strcmp(componentNode->name(), "BoxColliderComponent") == 0)
			{
				component = LoadBoxColliderComponent(componentNode, object);
			}
			else if (strcmp(componentNode->name(), "MovementComponent") == 0)
			{
				component = LoadMovementComponent(componentNode, object);
			}
			else if (strcmp(componentNode->name(), "AnimatorController") == 0)
			{
				component = LoadAnimatorController(componentNode, object);
			}
			else if (strcmp(componentNode->name(), "ScriptComponent") == 0)
			{
				component = LoadScriptComponent(componentNode, object);
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
		object->AddChild(LoadObject(childNode, scene));
	
	
	return object;
}

TransformComponent* SaveHandler::LoadTransformComponent(rapidxml::xml_node<>* node, GameObject* object)
{
	const float posX = std::stof(node->first_attribute("PosX")->value());
	const float posY = std::stof(node->first_attribute("PosY")->value());
	const float scaleX = std::stof(node->first_attribute("ScaleX")->value());
	const float scaleY = std::stof(node->first_attribute("ScaleY")->value());
	const float rot = std::stof(node->first_attribute("Rot")->value());

	TransformComponent* component = new TransformComponent(object);
	component->SetAttributes({ posX, posY }, { scaleX, scaleY }, rot);

	return component;
}

TextureComponent* SaveHandler::LoadTextureComponent(rapidxml::xml_node<>* node, GameObject* object)
{
	const std::string texturePath = node->first_attribute("TexturePath")->value();
	const float offsetX = std::stof(node->first_attribute("OffsetX")->value());
	const float offsetY = std::stof(node->first_attribute("OffsetY")->value());
	const int center = std::stoi(node->first_attribute("Center")->value());

	TextureComponent* component = new TextureComponent(object, texturePath);
	component->SetAttributes({ offsetX, offsetY }, center);

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
	component->SetAttributes({ offsetX, offsetY });

	return component;
}

RigidbodyComponent* SaveHandler::LoadRigidbodyComponent(rapidxml::xml_node<>* node, GameObject* object)
{
	const float density = std::stof(node->first_attribute("Density")->value());
	const float friction = std::stof(node->first_attribute("Friction")->value());
	const float restitution = std::stof(node->first_attribute("Restitution")->value());
	std::string type = node->first_attribute("Type")->value();

	RigidbodyComponent* component = new RigidbodyComponent(object);
	component->SetAttributes(type, density, friction, restitution);

	return component;
}

BoxColliderComponent* SaveHandler::LoadBoxColliderComponent(rapidxml::xml_node<>* node, GameObject* object)
{
	const float width = std::stof(node->first_attribute("Width")->value());
	const float height = std::stof(node->first_attribute("Height")->value());
	const int renderCollider = std::stoi(node->first_attribute("RenderCollider")->value());

	BoxColliderComponent* component = new BoxColliderComponent(object, object->GetRigidbody());
	component->SetAttributes(width, height, renderCollider);

	return component;
}

MovementComponent* SaveHandler::LoadMovementComponent(rapidxml::xml_node<>* node, GameObject* object)
{
	const float speed = std::stof(node->first_attribute("Speed")->value());
	const float maxSpeed = std::stof(node->first_attribute("MaxSpeed")->value());
	const float jumpStrength = std::stof(node->first_attribute("JumpStrength")->value());

	MovementComponent* component = new MovementComponent(object);
	component->SetAttributes(speed, maxSpeed, jumpStrength);

	return component;
}

AnimatorControllerComponent* SaveHandler::LoadAnimatorController(rapidxml::xml_node<>* node, GameObject* object)
{
	std::vector<Sprite*> pSprites;
	for (rapidxml::xml_node<>* spriteNode = node->first_node(); spriteNode != 0; spriteNode = spriteNode->next_sibling())
	{
		const std::string name = spriteNode->first_attribute("Name")->value();
		auto textureComp = LoadTextureComponent(spriteNode, object);
		const float spriteWidth = std::stof(spriteNode->first_attribute("SpriteWidth")->value());
		const float spriteHeight = std::stof(spriteNode->first_attribute("SpriteHeight")->value());
		const std::string transitionName = spriteNode->first_attribute("TransitionName")->value();
		const std::string texturePath = spriteNode->first_attribute("TexturePath")->value();
		const float timeBetweenFrames = std::stof(spriteNode->first_attribute("TimeBetweenFrames")->value());
		const float spacePerFrame = std::stof(spriteNode->first_attribute("SpacePerFrame")->value());
		const int rows = std::stoi(spriteNode->first_attribute("Rows")->value());
		const int columns = std::stoi(spriteNode->first_attribute("Columns")->value());

		Sprite* pSprite = new Sprite(object, name);
		pSprite->SetAttributes(textureComp, transitionName, texturePath, spriteWidth, spriteHeight, timeBetweenFrames, spacePerFrame, rows, columns);
		pSprites.push_back(pSprite);
	}

	AnimatorControllerComponent* component = new AnimatorControllerComponent(object);
	component->SetAttributes(pSprites);
	return component;
}

ScriptComponent* SaveHandler::LoadScriptComponent(rapidxml::xml_node<>* node, GameObject* object) 
{
	//Make this cleaner, ugly as fuck yoooh

	const std::string name = node->first_attribute("Name")->value();
	Script* script = GameObjectManager::GetInstance()->GetScript(name);

	ScriptComponent* component = new ScriptComponent(object);

	if (name == "PlayerScript")
	{
		PlayerScript* derivedScript{ new PlayerScript(*static_cast<PlayerScript*>(script)) };
		derivedScript->SetAttributes(node);
		derivedScript->SetGameObject(object);
		component->SetAttributes(derivedScript);
	}

	return component;
}
