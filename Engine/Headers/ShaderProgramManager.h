#pragma once
#include "Singleton.h"
#include "ShaderProgram.h"
#include "IManager.h"

namespace engine 
{
	class ShaderProgramManager : public Singleton <ShaderProgramManager>, public IManager<ShaderProgram>
	{
		friend Singleton<ShaderProgramManager>;
		ShaderProgramManager();
		~ShaderProgramManager();

	public:
		virtual void remove(std::string shadername) override;
		virtual ShaderProgram* get(std::string shadername) override;
		virtual void add(std::string shadername, ShaderProgram* shader) override;
	};
}
