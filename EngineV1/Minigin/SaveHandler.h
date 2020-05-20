#pragma once

class Scene;
class GameObject;
class TransformComponent;
class TextureComponent;
class TextComponent;

class SaveHandler
{
public:
	SaveHandler() = default;
	void Save(const std::vector<Scene*>& scenes);
	void Load(std::vector<Scene*>& scenes);

private:
	static const std::string m_FilePath;

	GameObject* LoadObject(rapidxml::xml_node<>* node);
	TransformComponent* LoadTransformComponent(rapidxml::xml_node<>* node, GameObject* object);
	TextureComponent* LoadTextureComponent(rapidxml::xml_node<>* node, GameObject* object);
	TextComponent* LoadTextComponent(rapidxml::xml_node<>* node, GameObject* object);
};

