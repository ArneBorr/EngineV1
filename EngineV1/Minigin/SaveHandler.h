#pragma once

class Scene;
class GameObject;
class TransformComponent;
class TextureComponent;
class TextComponent;
class RigidbodyComponent;
class BoxColliderComponent;
struct KeyboardButton;
class MovementComponent;
class AnimatorControllerComponent;

class SaveHandler
{
public:
	SaveHandler() = default;
	void SaveScenes(const std::vector<Scene*>& scenes);
	void LoadScenes(std::vector<Scene*>& scenes);
	void SaveInput(const std::map<std::string, std::vector<KeyboardButton*>>& actions);
	void LoadInput(std::map<std::string, std::vector<KeyboardButton*>>& actions, const std::map<int, KeyboardButton*>& keys);

private:
	static const std::string m_FilePathScenes;
	static const std::string m_FilePathInput;

	GameObject* LoadObject(rapidxml::xml_node<>* node, Scene* scene);
	TransformComponent* LoadTransformComponent(rapidxml::xml_node<>* node, GameObject* object);
	TextureComponent* LoadTextureComponent(rapidxml::xml_node<>* node, GameObject* object);
	TextComponent* LoadTextComponent(rapidxml::xml_node<>* node, GameObject* object);
	RigidbodyComponent* LoadRigidbodyComponent(rapidxml::xml_node<>* node, GameObject* object);
	BoxColliderComponent* LoadBoxColliderComponent(rapidxml::xml_node<>* node, GameObject* object);
	MovementComponent* LoadMovementComponent(rapidxml::xml_node<>* node, GameObject* object);
	AnimatorControllerComponent* LoadAnimatorController(rapidxml::xml_node<>* node, GameObject* object);
};

