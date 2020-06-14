#pragma once

struct Mix_Chunk;

class Effect2D
{
public:
	Mix_Chunk* GetSDLChunk() const;
	Effect2D(const std::string& pChunk);
	~Effect2D();

	Effect2D(const Effect2D&) = delete;
	Effect2D(Effect2D&&) = delete;
	Effect2D& operator= (const Effect2D&) = delete;
	Effect2D& operator= (const Effect2D&&) = delete;

private:
	Mix_Chunk* m_pChunk;
};

