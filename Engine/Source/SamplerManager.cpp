#include "..\Headers\pch.h"
#include "..\Headers\SamplerManager.h"

namespace engine
{
	SamplerManager::SamplerManager()
	{
		this->_objects = new std::unordered_map <std::string, Sampler*>();
	}
	SamplerManager::~SamplerManager()
	{
		for (auto it = this->_objects->begin(); it != this->_objects->end(); it++)
		{
			delete(it->second);
		}
		_objects->clear();
		delete _objects;
	}
	void SamplerManager::remove(std::string samplerName)
	{
		if (_objects->find(samplerName) != _objects->end())
		{
			this->_objects->erase(samplerName);
			return;
		}
		std::cerr << "Sampler with that name does not exists!" << std::endl;
	}
	Sampler* SamplerManager::get(std::string samplerName)
	{
		if (_objects->find(samplerName) != _objects->end())
		{
			return (*_objects)[samplerName];
		}
		std::cerr << "Sampler with that name does not exists!" << std::endl;
		return nullptr;
	}
	void SamplerManager::add(std::string samplerName, Sampler* sampler)
	{
		if (_objects->find(samplerName) != _objects->end())
		{
			std::cerr << "Sampler with that name already exists!" << std::endl;
			return;
		}
		(*_objects)[samplerName] = sampler;

	}
}