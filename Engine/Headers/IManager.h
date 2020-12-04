#pragma once

namespace engine
{
	template<typename T>
	class IManager
	{		
	public:
		std::unordered_map <std::string, T*>* _objects = 0;
		inline std::unordered_map<std::string, T*> getObjects() { return _objects; }
		virtual void remove(std::string name) = 0;
		virtual T* get(std::string rname) = 0;
		virtual void add(std::string shadername, T* object) = 0;		
	};
}