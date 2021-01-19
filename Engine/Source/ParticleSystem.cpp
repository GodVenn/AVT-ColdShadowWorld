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
			particle.Position += (particle.Velocity + cameraMovement) * (float)ts;


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

		//Reset camera movement
		cameraMovement = Vec3(0.0f);

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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		ParticleShader->bind();
		text->updateShader(ParticleShader);
		cam->draw();
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, POOL_SIZE);
		ParticleShader->unbind();
		glDisable(GL_BLEND);
	}

	void ParticleSystem::AddParticle(const Particle& particleProps, Camera* cam)
	{
		Particle& particle = Particle_Pool[Particle_Pool_Index];
		particle.Active = true;
		particle.Position = particleProps.Position;

		//Randomly distribute particles in a circle around the position
		float t = 2 * M_PI * ((rand() % 100) / 100.0f);
		float u = ((rand() % 100) / 100.0f) + ((rand() % 100) / 100.0f);
		float r = u > 1.0f ? 2 - u : u;
		particle.Position.x = cam->eye.x + (r * cos(t))* circleSize;
		particle.Position.y += cam->eye.y;
		particle.Position.z = cam->eye.z + (r * sin(t))* circleSize;


		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x = ((rand() % 100) / 100.0f - 0.5f) / 5.0f;
		particle.Velocity.y = -0.5f;
		particle.Velocity.z = ((rand() % 100) / 100.0f - 0.5f) / 5.0f;

		particle.Color = particleProps.Color;

		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;
		particle.Size = particleProps.Size;

		Particle_Pool_Index = ++Particle_Pool_Index % (POOL_SIZE);

		//if(Particle_Pool_Index % 100 == 0)
			//std::cout << "index: " << Particle_Pool_Index << std::endl;
	}

	void ParticleSystem::SetCameraMovement(const Vec3& movement)
	{
		cameraMovement = movement;
	}

}
