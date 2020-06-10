#pragma once
struct SDL_Texture;

	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D
	{
	public:
		SDL_Texture* GetSDLTexture() const;
		explicit Texture2D(SDL_Texture* texture, const std::string& name = " ");
		~Texture2D();

		Texture2D(const Texture2D & other) noexcept;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;
	private:
		SDL_Texture* m_pTexture;
		std::string m_Name;
	};

