#include "../Headers/pch.h"
#include "../Headers/ShaderProgramManager.h"
#include "../Headers/Constants.h"

namespace engine
{
	ShaderProgramManager::ShaderProgramManager()
	{
		this->_objects = new std::unordered_map <std::string, ShaderProgram*>();
	}

	ShaderProgramManager::~ShaderProgramManager()
	{
		for (auto it = this->_objects->begin(); it != this->_objects->end(); ++it) {
			delete(it->second);
		}
		_objects->clear();
		delete _objects;
	}
	void ShaderProgramManager::remove(std::string shaderName)
	{
		if (_objects->find(shaderName) != _objects->end())
		{
			this->_objects->erase(shaderName);
			return;
		}
		
		std::cerr << "Shader with that name does not exists!" << std::endl;
	}

	ShaderProgram* ShaderProgramManager::get(std::string shaderName)
	{
		if (_objects->find(shaderName) != _objects->end())
		{
			return (*_objects)[shaderName];			
		}
		std::cerr << "Shader with that name does not exists!" << std::endl;
		return nullptr;
	}

	void ShaderProgramManager::add(std::string shaderName, ShaderProgram* shader)
	{
		if (_objects->find(shaderName) != _objects->end())
		{
			std::cerr << "Shader with that name already exists!" << std::endl;	
			return;
		}
		(*_objects)[shaderName] = shader;
	}

}