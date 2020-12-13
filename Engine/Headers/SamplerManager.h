#pragma once

#include "..\Headers\Singleton.h"
#include "..\Headers\Texture.h"
#include "..\Headers\IManager.h"

namespace engine
{
	class SamplerManager : public Singleton<SamplerManager>, public IManager<Sampler>
	{
	public:
		virtual void remove(std::string samplerName) override;
		virtual Sampler* get(std::string samplerName) override;
		virtual void add(std::string samplerName, Sampler* sampler) override;

	private:
		friend Singleton<SamplerManager>;

		SamplerManager();
		~SamplerManager();
	};


}