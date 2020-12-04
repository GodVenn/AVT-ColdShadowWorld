#pragma once

namespace engine 
{
	template<typename T>
	class Singleton
	{

	protected:
		static T* instance;
		Singleton() { instance = static_cast<T*> (this); };


	public:
		static T* getInstance();
		static void freeInstance();

		Singleton& operator = (Singleton& other) = delete;
		Singleton(Singleton& other) = delete;

	};

	//definition of template methods
	template<typename T>
	typename T* Singleton<T>::instance = nullptr;

	template<typename T>
	T* Singleton<T>::getInstance()
	{
		if (instance == nullptr)
		{
			Singleton<T>::instance = new T();
		}
		return instance;
	}

	template<typename T>
	void Singleton<T>::freeInstance()
	{
		delete Singleton<T>::instance;
		Singleton<T>::instance = nullptr;
	}
}

