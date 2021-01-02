#pragma once
#include "Vector.h"
#include "Matrix.h"
#include "ShaderProgram.h"
#include "ShaderProgramManager.h"
#include "Constants.h"
#include "Camera.h"

#define POOL_SIZE 100
#define OFFSET_ARRAY 7

namespace engine
{
	struct Particle
	{
		Vec3 Position = Vec3(0.0f);
		Vec3 Velocity = Vec3(0.0f);
		Vec4 Color = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		float Rotation = 0.0f;
		float Size = 0.5f;

		float LifeTime = 5.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};

	class ParticleSystem : public Singleton <ParticleSystem>
	{
	public:
		friend Singleton<ParticleSystem>;
		ParticleSystem();
		~ParticleSystem();
		void OnUpdate(double ts, Camera* cam);
		void Render(Camera * cam);

		void AddParticle(const Particle& particleProps);
	private:
		std::vector<Particle> Particle_Pool;
		uint32_t Particle_Pool_Index = 0;

		unsigned int VBO, VAO;
		ShaderProgram* ParticleShader;
	};
}