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
class ScriptComponent;
class FSMComponent;
class Behaviour;
struct MusicObject;
struct EffectObject;
struct KeyboardAction;

class SaveHandler
{
public:
	SaveHandler() = default;
	void SavePrefab(GameObject* pObject);
	void LoadPrefabNames(std::vector<std::string>& prefabs);
	GameObject* LoadPrefab(Scene* pScene, const std::string& name);
	void ErasePrefab(const std::string& prefabs);
	void SaveScenes(const std::vector<Scene*>& pScenes);
	void LoadScenes(std::vector<Scene*>& pScenes);
	void SaveInput(const std::vector<KeyboardAction*>& pActions);
	void LoadInput(std::vector<KeyboardAction*>& pActions, const std::map<int, KeyboardButton*>& pKeys);
	void SaveSound(const std::vector<MusicObject*>& pSounds, const std::vector<EffectObject*>& pEffects);
	void LoadSound(std::vector<MusicObject*>& pSounds, std::vector<EffectObject*>& pEffects);

private:
	static const std::string m_FilePathScenes;
	static const std::string m_FilePathInput;
	static const std::string m_FilePathPrefabs;
	static const std::string m_FilePathSound;

	GameObject* LoadObject(rapidxml::xml_node<>* pNode, Scene* pScene);
	TransformComponent* LoadTransformComponent(rapidxml::xml_node<>* pNode, GameObject* pObject);
	TextureComponent* LoadTextureComponent(rapidxml::xml_node<>* pNode, GameObject* pObject);
	TextComponent* LoadTextComponent(rapidxml::xml_node<>* pNode, GameObject* pObject);
	RigidbodyComponent* LoadRigidbodyComponent(rapidxml::xml_node<>* pNode, GameObject* pObject);
	BoxColliderComponent* LoadBoxColliderComponent(rapidxml::xml_node<>* pNode, GameObject* pObject);
	MovementComponent* LoadMovementComponent(rapidxml::xml_node<>* pNode, GameObject* pObject);
	AnimatorControllerComponent* LoadAnimatorController(rapidxml::xml_node<>* pNode, GameObject* pObject);
	ScriptComponent* LoadScriptComponent(rapidxml::xml_node<>* pNode, GameObject* pObject);
	FSMComponent* LoadFSMComponent(rapidxml::xml_node<>* pNode, GameObject* pObject);
};

