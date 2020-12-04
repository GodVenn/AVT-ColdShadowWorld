#include "../Headers/pch.h"
#include "../Headers/Simulation.h"

namespace engine
{
	Simulation::Simulation()
	{
		
	}
	Simulation::~Simulation()
	{
		this->objects.clear();
	}
	void Simulation::add(IUpdatable* object)
	{
		this->objects.push_back(object);
	}

	void Simulation::update(float deltaTime)
	{
		for (auto& object : objects) 
		{
			object->onUpdate(deltaTime);
		}
	}
}