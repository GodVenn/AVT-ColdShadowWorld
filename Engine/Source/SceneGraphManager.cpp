#include "../Headers/pch.h"
#include "../Headers/SceneGraphManager.h"

namespace engine 
{
	SceneGraphManager::SceneGraphManager()
	{
		this->_objects = new std::unordered_map <std::string, SceneGraph*>();
	}

	SceneGraphManager::~SceneGraphManager()
	{
		for (auto it = this->_objects->begin(); it != this->_objects->end(); ++it) {
			delete(it->second);
		}
		_objects->clear();
		delete _objects;
	}

	void SceneGraphManager::remove(std::string sceneName)
	{
		if (_objects->find(sceneName) != _objects->end())
		{
			this->_objects->erase(sceneName);
			return;
		}
		std::cerr << "Scene with that name does not exists!" << std::endl;
		
	}

	SceneGraph* SceneGraphManager::get(std::string sceneName)
	{
		if (_objects->find(sceneName) != _objects->end())
		{
			return (*_objects)[sceneName];
		}
		std::cerr << "Scene with that name does not exists!" << std::endl;
		return nullptr;
	}

	void SceneGraphManager::add(std::string sceneName, SceneGraph* scene)
	{
		if (_objects->find(sceneName) != _objects->end())
		{
			std::cerr << "Scene with that name already exists!" << std::endl;
			return;
		}
		(*_objects)[sceneName] = scene;
	}
}