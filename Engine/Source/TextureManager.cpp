#include "../Headers/pch.h"

#include "../Headers/TextureManager.h"

namespace engine
{
	TextureManager::TextureManager()
	{
		this->_objects = new std::unordered_map <std::string, Texture*>();
	}
	TextureManager::~TextureManager()
	{
		for (auto it = this->_objects->begin(); it != this->_objects->end(); it++)
		{
			delete(it->second);
		}
		_objects->clear();
		delete _objects;
	}
	void TextureManager::remove(std::string textureName)
	{
		if (_objects->find(textureName) != _objects->end())
		{
			this->_objects->erase(textureName);
			return;
		}
		std::cerr << "Texture with that name does not exists!" << std::endl;
	}
	Texture* TextureManager::get(std::string textureName)
	{
		if (_objects->find(textureName) != _objects->end())
		{
			return (*_objects)[textureName];
		}
		std::cerr << "Texture with that name does not exists!" << std::endl;
		return nullptr;
	}
	void TextureManager::add(std::string textureName, Texture* texture)
	{
		if (_objects->find(textureName) != _objects->end())
		{
			std::cerr << "Texture with that name already exists!" << std::endl;
			return;
		}
		(*_objects)[textureName] = texture;

	}
}