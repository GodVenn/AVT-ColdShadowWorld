#pragma once

#include "Singleton.h"

namespace engine
{
	///////////////////////////////////////////////////////////////////////
	class IUpdatable;
	class Simulation;
	/////////////////////////////////////////////////////////////////////// IUpdatable
	class IUpdatable
	{
	public:
		virtual void onUpdate(float deltaTime) = 0;
	};
	/////////////////////////////////////////////////////////////////////// Simulation
	class Simulation: public Singleton<Simulation>
	{
	public:
		void add(IUpdatable* object);
		void update(float deltaTime);

	private:

		Simulation();
		~Simulation();
		friend class Singleton<Simulation>;
		std::vector<IUpdatable*> objects;

	};

}