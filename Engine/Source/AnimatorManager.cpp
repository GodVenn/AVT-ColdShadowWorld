#include "../Headers/pch.h"
#include "../Headers/AnimatorManager.h"

namespace engine 
{

	AnimatorManager::AnimatorManager()
	{
		this->_objects = new std::unordered_map <std::string, Animation*>();
	}


	AnimatorManager::~AnimatorManager()
	{
		for (auto it = this->_objects->begin(); it != this->_objects->end(); it++) 
		{
			delete(it->second);
		}
		_objects->clear();
		delete _objects;
	}
	void AnimatorManager::remove(std::string animatorName)
	{
		if (_objects->find(animatorName) != _objects->end())
		{
			this->_objects->erase(animatorName);
			return;
		}
		std::cerr << "Animator with that name does not exists!" << std::endl;
	}
	Animation* AnimatorManager::get(std::string animatorName)
	{
		if (_objects->find(animatorName) != _objects->end())
		{
			return (*_objects)[animatorName];
		}
		std::cerr << "Animator with that name does not exists!" << std::endl;
		return nullptr;
	}
	void AnimatorManager::add(std::string animatorName, Animation* animator)
	{
		if (_objects->find(animatorName) != _objects->end())
		{
			std::cerr << "Animator with that name already exists!" << std::endl;
			return;
		}
		(*_objects)[animatorName] = animator;

	}
}