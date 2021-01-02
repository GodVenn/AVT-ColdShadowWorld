#include "../Headers/pch.h"
#include "../Headers/ParticleSystem.h"

namespace engine {

	ParticleSystem::ParticleSystem()
	{
		Particle_Pool.resize(POOL_SIZE);
		ParticleShader = ShaderProgramManager::getInstance()->get("Particles");
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
	}

	ParticleSystem::~ParticleSystem()
	{
	}

	void ParticleSystem::OnUpdate(double ts, Camera* cam)
	{
		int counter = 0;
		float particles[POOL_SIZE * OFFSET_ARRAY] = { 0.0f };
		for (auto& particle : Particle_Pool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= (float)ts;
			particle.Position += particle.Velocity * (float)ts;


			int k = counter * OFFSET_ARRAY;
			Vec4 color = particle.Color;
			Vec3 pos = particle.Position;
			particles[k] = pos.x;
			particles[k + 1] = pos.y;
			particles[k + 2] = pos.z;
			particles[k + 3] = color.x;
			particles[k + 4] = color.y;
			particles[k + 5] = color.z;
			particles[k + 6] = 1.0f;

			counter++;
		}

		std::size_t size = sizeof(float) * (std::size_t)(((std::size_t)POOL_SIZE) * (std::size_t)OFFSET_ARRAY);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, size, &particles[0], GL_STREAM_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, OFFSET_ARRAY * sizeof(float), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, OFFSET_ARRAY * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		Render(cam);
	}

	void ParticleSystem::Render(Camera* cam)
	{
		ParticleShader->bind();
		cam->draw();
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 100);
		ParticleShader->unbind();

	}

	void ParticleSystem::AddParticle(const Particle& particleProps)
	{
		Particle& particle = Particle_Pool[Particle_Pool_Index];
		particle.Active = true;
		particle.Position = particleProps.Position;

		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x = ((rand() % 100) / 100.0f - 0.5f);
		particle.Velocity.y = ((rand() % 100) / 100.0f - 0.5f);
		particle.Velocity.z = ((rand() % 100) / 100.0f - 0.5f);

		particle.Color = particleProps.Color;

		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;
		particle.Size = particleProps.Size;

		Particle_Pool_Index = ++Particle_Pool_Index % (POOL_SIZE);
		std::cout << "index: " << Particle_Pool_Index << std::endl;
	}

}
