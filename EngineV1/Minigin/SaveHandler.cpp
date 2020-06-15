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
#include "AllowOneWay.h"
#include "Behaviours.h"
#include "SoundManager.h"

const std::string SaveHandler::m_FilePathScenes{ "Data/SaveScenes.xml" };
const std::string SaveHandler::m_FilePathInput{ "Data/SaveInput.xml" };
const std::string SaveHandler::m_FilePathPrefabs{ "Data/SavePrefabs.xml" };
const std::string SaveHandler::m_FilePathSound{ "Data/SaveSound.xml" };

using namespace rapidxml;

void SaveHandler::SavePrefab(GameObject* pObject)
{
	xml_document<>* pDoc = new xml_document<>();

	xml_node<>* prefabNode = pDoc->allocate_node(node_element, "Prefab");
	pDoc->append_node(prefabNode);
	pObject->SaveAttributes(pDoc, prefabNode);

	std::ofstream file_stored;
	file_stored.open(m_FilePathPrefabs, std::fstream::app);
	file_stored << *pDoc;
	file_stored.close();

	delete pDoc;
	pDoc = nullptr;
}

void SaveHandler::LoadPrefabNames(std::vector<std::string>& prefabs)
{
	xml_document<>* pDoc = new xml_document<>();
	std::ifstream file(m_FilePathPrefabs);

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content(buffer.str());
	pDoc->parse<0>(&content[0]);
	
	for (auto prefabNode = pDoc->first_node("Prefab"); prefabNode; prefabNode = prefabNode->next_sibling())
		prefabs.push_back(prefabNode->first_node("GameObject")->first_attribute("Name")->value());

	delete pDoc;
	pDoc = nullptr;
}

GameObject* SaveHandler::LoadPrefab(Scene* pScene, const std::string& name)
{
	xml_document<>* pDoc = new xml_document<>();
	std::ifstream file(m_FilePathPrefabs);

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content(buffer.str());
	pDoc->parse<0>(&content[0]);

	for (auto prefabNode = pDoc->first_node("Prefab"); prefabNode; prefabNode = prefabNode->next_sibling())
	{
		auto gameObjectNode = prefabNode->first_node("GameObject");
		if (gameObjectNode->first_attribute("Name")->value() == name)
		{		
			auto pObject = LoadObject(gameObjectNode, pScene);
			delete pDoc;
			pDoc = nullptr;
			return pObject;
		}
	}

	delete pDoc;
	pDoc = nullptr;
	return nullptr;
}

void SaveHandler::ErasePrefab(const std::string& name)
{
	xml_document<>* pDocSave = new xml_document<>();

	xml_document<>* pDocRead = new xml_document<>();
	std::ifstream file(m_FilePathPrefabs);

	//Read
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content(buffer.str());
	pDocRead->parse<0>(&content[0]);

	for (auto prefabNode = pDocRead->first_node("Prefab"); prefabNode; prefabNode = prefabNode->next_sibling())
	{
		auto gameObjectNode = prefabNode->first_node("GameObject");
		if (gameObjectNode->first_attribute("Name")->value() == name) // Filter deleted prefab
			continue;

		xml_node<>* temp = pDocSave->allocate_node(node_element, "Prefab");
		pDocRead->clone_node(prefabNode, temp);
		pDocSave->append_node(temp);
	}

	//Save again
	std::ofstream file_stored(m_FilePathPrefabs);
	file_stored << *pDocSave;
	file_stored.close();

	delete pDocRead;
	pDocRead = nullptr;
	delete pDocSave;
	pDocSave = nullptr;
}

void SaveHandler::SaveScenes(const std::vector<Scene*>& pScenes)
{
	xml_document<>* pDoc{ new xml_document<>() };

	xml_node<>* pRoot = pDoc->allocate_node(node_element, "Root");
	pRoot->append_attribute(pDoc->allocate_attribute("Game", "BubbleBobble"));
	pDoc->append_node(pRoot);

	//Scenes
	//***********
	for (Scene* pScene : pScenes)
	{
		xml_node<>* pSceneNode = pDoc->allocate_node(node_element, "Scene");
		pSceneNode->append_attribute(pDoc->allocate_attribute("Name", pScene->GetName().c_str()));
		auto pOverhead = pScene->GetOverhead();
		if (pOverhead)
		{
			xml_node<>* overheadNode = pDoc->allocate_node(node_element, "Overhead");
			pOverhead->SaveAttributes(pDoc, overheadNode);
			pSceneNode->append_node(overheadNode);
		}
		pRoot->append_node(pSceneNode);

		//Objects
		//***********
		for (GameObject* pObject : pScene->GetGameObjects())
			pObject->SaveAttributes(pDoc, pSceneNode);
	}

	std::ofstream file_stored(m_FilePathScenes);
	file_stored << *pDoc;
	file_stored.close();

	delete pDoc;
	pDoc = nullptr;
}

void SaveHandler::LoadScenes(std::vector<Scene*>& scenes)
{
	xml_document<>* pDoc{ new xml_document<>() };
	std::ifstream file(m_FilePathScenes);

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content(buffer.str());
	pDoc->parse<0>(&content[0]);
	auto rootNode = pDoc->first_node();

	//Scenes
	//***********
	for (auto sceneNode = rootNode->first_node("Scene"); sceneNode; sceneNode = sceneNode->next_sibling())
	{
		Scene* pScene = new Scene(sceneNode->first_attribute("Name")->value());
		scenes.push_back(pScene);

		auto overheadNode = sceneNode->first_node("Overhead");
		if (overheadNode != 0)
		{
			pScene->SetOVerhead(LoadObject(overheadNode->first_node("GameObject"), pScene));
		}

		//Objects
		//***********
		for (auto objectNode = sceneNode->first_node("GameObject"); objectNode; objectNode = objectNode->next_sibling())
			pScene->AddChild(LoadObject(objectNode, pScene));
	}

	delete pDoc;
	pDoc = nullptr;
}

void SaveHandler::SaveInput(const std::vector<KeyboardAction*>& pActions)
{
	xml_document<>* pDoc{ new xml_document<>() };

	xml_node<>* pRoot = pDoc->allocate_node(node_element, "Root");
	pRoot->append_attribute(pDoc->allocate_attribute("Game", "BubbleBobble"));
	pDoc->append_node(pRoot);

	//Keys
	//***********
	for (const auto& action : pActions)
	{
		xml_node<>* actionNode = pDoc->allocate_node(node_element, "Action");
		actionNode->append_attribute(pDoc->allocate_attribute("Name", action->name.c_str()));

		for (auto key : action->keys)
		{
			xml_node<>* keyNode = pDoc->allocate_node(node_element, "Key");
			keyNode->append_attribute(pDoc->allocate_attribute("Name", key.second->name.c_str()));
			keyNode->append_attribute(pDoc->allocate_attribute("ID", IntToXMLChar(pDoc, key.second->id)));
			keyNode->append_attribute(pDoc->allocate_attribute("Player", IntToXMLChar(pDoc, (int)key.first)));
			actionNode->append_node(keyNode);
		}
			
		pRoot->append_node(actionNode);
	}

	std::ofstream file_stored(m_FilePathInput);
	file_stored << *pDoc;
	file_stored.close();

	delete pDoc;
	pDoc = nullptr;
}

void SaveHandler::LoadInput(std::vector<KeyboardAction*>& pActions, const std::map<int, KeyboardButton*>& pKeys)
{
	xml_document<>* pDoc{ new xml_document<>() };
	std::ifstream file(m_FilePathInput);
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content(buffer.str());
	pDoc->parse<0>(&content[0]);

	auto rootNode = pDoc->first_node();

	//Actions
	//***********
	
	for (auto actionNode = rootNode->first_node("Action"); actionNode; actionNode = actionNode->next_sibling())
	{
		KeyboardAction* action{ new KeyboardAction(actionNode->first_attribute("Name")->value()) };

		//Keys
		//***********
		for (auto keyNode = actionNode->first_node("Key"); keyNode; keyNode = keyNode->next_sibling())
		{
			const int id = std::stoi(keyNode->first_attribute("ID")->value());
			const int player = std::stoi(keyNode->first_attribute("Player")->value());
			action->keys.push_back({ (PlayerAction)player, pKeys.at(id) });
		}

		pActions.push_back(action);
	}

	delete pDoc;
	pDoc = nullptr;
}

void SaveHandler::SaveSound(const std::vector<MusicObject*>& sounds, const std::vector<EffectObject*>& effects)
{
	xml_document<>* doc{ new xml_document<>() };

	xml_node<>* root = doc->allocate_node(node_element, "Root");
	root->append_attribute(doc->allocate_attribute("Game", "BubbleBobble"));
	doc->append_node(root);

	xml_node<>* soundTitleNode = doc->allocate_node(node_element, "Sounds");
	root->append_node(soundTitleNode);
	for (auto sound : sounds)
	{
		xml_node<>* soundNode = doc->allocate_node(node_element, "Sound");
		soundNode->append_attribute(doc->allocate_attribute("ID", sound->id));
		soundNode->append_attribute(doc->allocate_attribute("Path", sound->path));
		soundTitleNode->append_node(soundNode);
	}

	xml_node<>* effectTitleNode = doc->allocate_node(node_element, "Effects");
	root->append_node(effectTitleNode);
	for (auto effect : effects)
	{
		xml_node<>* effectNode = doc->allocate_node(node_element, "Effect");
		effectNode->append_attribute(doc->allocate_attribute("ID", effect->id));
		effectNode->append_attribute(doc->allocate_attribute("Path", effect->path));
		effectTitleNode->append_node(effectNode);
	}

	std::ofstream file_stored(m_FilePathSound);
	file_stored << *doc;
	file_stored.close();

	delete doc;
	doc = nullptr;
}

void SaveHandler::LoadSound(std::vector<MusicObject*>& music, std::vector<EffectObject*>& effects)
{
	xml_document<>* doc{ new xml_document<>() };
	std::ifstream file(m_FilePathSound);
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content(buffer.str());
	doc->parse<0>(&content[0]);
	auto rootNode = doc->first_node();

	auto soundsTitleNode = rootNode->first_node("Sounds");
	for (auto soundNode = soundsTitleNode->first_node("Sound"); soundNode; soundNode = soundNode->next_sibling())
	{
		std::string id = soundNode->first_attribute("ID")->value();
		std::string path = soundNode->first_attribute("Path")->value();

		MusicObject* pMusic = new MusicObject(id, path);
		music.push_back(pMusic);
	}

	auto effectsTitleNode = rootNode->first_node("Effects");
	for (auto effectNode = effectsTitleNode->first_node("Effect"); effectNode; effectNode = effectNode->next_sibling())
	{
		std::string id = effectNode->first_attribute("ID")->value();
		std::string path = effectNode->first_attribute("Path")->value();

		EffectObject* pEffect = new EffectObject(id, path);
		effects.push_back(pEffect);
	}

	delete doc;
	doc = nullptr;
}

GameObject* SaveHandler::LoadObject(rapidxml::xml_node<>* node, Scene* scene)
{
	GameObject* object = new GameObject(node->first_attribute("Name")->value());
	object->SetScene(scene);

	//Tags
	//***********
	xml_node<>* tagsNode = node->first_node("Tags");
	for (xml_attribute<>* tag = tagsNode->first_attribute(); tag != 0; tag = tag->next_attribute())
		object->AddTag(tag->value());

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
			else if (strcmp(componentNode->name(), "FSMComponent") == 0)
			{
				component = LoadFSMComponent(componentNode, object);
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
	const bool noRot = std::stoi(node->first_attribute("NoRotation")->value());
	const std::string type = node->first_attribute("Type")->value();
	BoxColliderComponent* pGroundDetector = nullptr;

	auto pGroundDetectorNode = node->first_node("GroundDetector");
	if (pGroundDetectorNode != 0)
		pGroundDetector = LoadBoxColliderComponent(pGroundDetectorNode, object);

	RigidbodyComponent* component = new RigidbodyComponent(object);
	component->SetAttributes(pGroundDetector, type, noRot);

	return component;
}

BoxColliderComponent* SaveHandler::LoadBoxColliderComponent(rapidxml::xml_node<>* node, GameObject* object)
{
	const float width = std::stof(node->first_attribute("Width")->value());
	const float height = std::stof(node->first_attribute("Height")->value());
	const float offsetX = std::stof(node->first_attribute("OffsetX")->value());
	const float offsetY = std::stof(node->first_attribute("OffsetY")->value());
	const int renderCollider = std::stoi(node->first_attribute("RenderCollider")->value());
	const int collGroup = std::stoi(node->first_attribute("CollGroup")->value());

	//Was not possible with loop because of rapidxml not reading local numbers :*
	std::vector<bool> ignoreGroups;
	ignoreGroups.resize(5);
	ignoreGroups[0] = std::stoi(node->first_attribute("IgnoreGr0")->value());
	ignoreGroups[1] = std::stoi(node->first_attribute("IgnoreGr1")->value());
	ignoreGroups[2] = std::stoi(node->first_attribute("IgnoreGr2")->value());
	ignoreGroups[3] = std::stoi(node->first_attribute("IgnoreGr3")->value());
	ignoreGroups[4] = std::stoi(node->first_attribute("IgnoreGr4")->value());

	const float density = std::stof(node->first_attribute("Density")->value());
	const float friction = std::stof(node->first_attribute("Friction")->value());
	const float restitution = std::stof(node->first_attribute("Restitution")->value());
	const int isSensor = std::stoi(node->first_attribute("IsSensor")->value());

	BoxColliderComponent* component = new BoxColliderComponent(object);
	component->SetAttributes(ignoreGroups, { offsetX , offsetY }, width, height, density, friction, restitution, collGroup, renderCollider, isSensor);

	return component;
}

MovementComponent* SaveHandler::LoadMovementComponent(rapidxml::xml_node<>* node, GameObject* object)
{
	const float speed = std::stof(node->first_attribute("Speed")->value());
	const float jumpStrength = std::stof(node->first_attribute("JumpStrength")->value());

	MovementComponent* component = new MovementComponent(object);
	component->SetAttributes(speed, jumpStrength);

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
		const std::string texturePath = spriteNode->first_attribute("TexturePath")->value();
		const float timeBetweenFrames = std::stof(spriteNode->first_attribute("TimeBetweenFrames")->value());
		const float spacePerFrame = std::stof(spriteNode->first_attribute("SpacePerFrame")->value());
		const int rows = std::stoi(spriteNode->first_attribute("Rows")->value());
		const int columns = std::stoi(spriteNode->first_attribute("Columns")->value());

		Sprite* pSprite = new Sprite(object, name);
		pSprite->SetAttributes(textureComp, texturePath, spriteWidth, spriteHeight, timeBetweenFrames, spacePerFrame, rows, columns);
		pSprites.push_back(pSprite);
	}

	AnimatorControllerComponent* component = new AnimatorControllerComponent(object);
	component->SetAttributes(pSprites);
	return component;
}

ScriptComponent* SaveHandler::LoadScriptComponent(rapidxml::xml_node<>* node, GameObject* object) 
{
	ScriptComponent* component = new ScriptComponent(object);

	auto nameAttribute = node->first_attribute("Name");
	if (nameAttribute != 0)
	{
		Script* script = GameObjectManager::GetInstance()->CreateScript(nameAttribute->value());
		component->SetAttributes(script, node);
	}
	
	return component;
}

FSMComponent* SaveHandler::LoadFSMComponent(rapidxml::xml_node<>* node, GameObject* object)
{
	FSMComponent* component = new FSMComponent(object);

	component->SetStartingState(std::stoi(node->first_attribute("StartingState")->value()));

	auto childNodeSprites = node->first_node();

	//Sprites
	std::vector<Sprite*> sprites;
	for (rapidxml::xml_node<>* spriteNode = childNodeSprites->first_node(); spriteNode != 0; spriteNode = spriteNode->next_sibling())
	{
		const std::string name = spriteNode->first_attribute("Name")->value();
		auto textureComp = LoadTextureComponent(spriteNode, object);
		const float spriteWidth = std::stof(spriteNode->first_attribute("SpriteWidth")->value());
		const float spriteHeight = std::stof(spriteNode->first_attribute("SpriteHeight")->value());
		const std::string texturePath = spriteNode->first_attribute("TexturePath")->value();
		const float timeBetweenFrames = std::stof(spriteNode->first_attribute("TimeBetweenFrames")->value());
		const float spacePerFrame = std::stof(spriteNode->first_attribute("SpacePerFrame")->value());
		const int rows = std::stoi(spriteNode->first_attribute("Rows")->value());
		const int columns = std::stoi(spriteNode->first_attribute("Columns")->value());

		Sprite* pSprite = new Sprite(object, name);
		pSprite->SetAttributes(textureComp, texturePath, spriteWidth, spriteHeight, timeBetweenFrames, spacePerFrame, rows, columns);
		sprites.push_back(pSprite);
	}
	component->SetSprites(sprites);

	//Behaviours
	auto childNodeBehaviours = childNodeSprites->next_sibling();
	std::vector<Behaviour*> behaviours;
	for (rapidxml::xml_node<>* behaviourNode = childNodeBehaviours->first_node(); behaviourNode != 0; behaviourNode = behaviourNode->next_sibling())
	{
		const std::string name = behaviourNode->first_attribute("Name")->value();
		auto behaviour = GameObjectManager::GetInstance()->CreateBehaviour(name);
		behaviour->SetFSM(component);
		behaviour->SetGameObject(object);
		behaviours.push_back(behaviour);
	}
	component->SetBehaviours(behaviours);
	component->SetAttributes(childNodeBehaviours);
	return component;
}
