#include "../Headers/pch.h"
#include "../Headers/MeshManager.h"


namespace engine
{
	MeshManager::MeshManager()
	{
		this->_objects = new std::unordered_map <std::string, Mesh*>();
	}

	MeshManager::~MeshManager()
	{
		for (auto it = this->_objects->begin(); it != this->_objects->end(); it++) 
		{
			delete(it->second);
		}
		_objects->clear();
		delete _objects;
	}
	void MeshManager::remove(std::string meshName)
	{
		if (_objects->find(meshName) != _objects->end())
		{
			this->_objects->erase(meshName);
			return;
		}
		std::cerr << "Mesh with that name does not exists!" << std::endl;
		
	}

	Mesh* MeshManager::get(std::string meshName)
	{
		if (_objects->find(meshName) != _objects->end())
		{
			return (*_objects)[meshName];
		}
		std::cerr << "Mesh with that name does not exists!" << std::endl;
		return nullptr;
	}

	void MeshManager::add(std::string meshName, Mesh* mesh)
	{
		if (_objects->find(meshName) != _objects->end())
		{
			std::cerr << "Scene with that name already exists!" << std::endl;
			return;
		}
		(*_objects)[meshName] = mesh;
	}
}