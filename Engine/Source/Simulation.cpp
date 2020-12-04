#include "../Headers/pch.h"
#include "../Headers/Simulation.h"
#include "../Headers/gl.h"

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
		for (auto& o : objects) 
		{
			o->onUpdate(deltaTime);
		}
	}
}