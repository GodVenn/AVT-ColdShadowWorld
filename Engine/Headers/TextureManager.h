#pragma once

#include "../Headers/Singleton.h"
#include "../Headers/Texture.h"
#include "../Headers/IManager.h"

namespace engine
{
	class TextureManager : public Singleton<TextureManager>, public IManager<Texture>
	{
	public:
		virtual void remove(std::string textureName) override;
		virtual Texture* get(std::string textureName) override;
		virtual void add(std::string textureName, Texture* texture) override;

	private:
		friend Singleton<TextureManager>;

		TextureManager();
		~TextureManager();
	};


}