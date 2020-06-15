#pragma once

struct _TTF_Font;

class Font
{
public:
	_TTF_Font* GetFont() const;
	explicit Font(const std::string& fullPath, unsigned int size);
	~Font();

	const std::string& GetPath() const { return m_Path; };
	int GetSize() const { return m_Size; };

	Font(const Font &) = delete;
	Font(Font &&) = delete;
	Font & operator= (const Font &) = delete;
	Font & operator= (const Font &&) = delete;
private:
	_TTF_Font* m_Font = nullptr;
	std::string m_Path = {};
	unsigned int m_Size = {};
};

