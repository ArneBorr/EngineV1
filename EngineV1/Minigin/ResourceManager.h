#pragma once
#include "Singleton.h"


class Texture2D;
class Font;
class Music2D;
class Effect2D;

class ResourceManager final : public Singleton<ResourceManager>
{
public:
	~ResourceManager() = default;
	void Init(const std::string& data);
	Texture2D* LoadTexture(const std::string& file) const;
	Font* LoadFont(const std::string& file, unsigned int size) const;
	Music2D* LoadMusic(const std::string& path);
	Effect2D* LoadSoundEffect(const std::string& path);

private:
	friend class Singleton<ResourceManager>;
	ResourceManager() = default;
	std::string m_DataPath;
	std::string m_DefaultTexturePath;
};

