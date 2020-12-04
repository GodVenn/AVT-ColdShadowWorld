#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace engine
{
	class ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram();

		const void load(const std::string& vertexSrc, const std::string& fragmentSrc);
		const void link();
		const void create();
		

		const void bind();
		const void unbind();

		const void addAttribute(const GLuint index, const std::string& name);
		const void addUniformBlock(const std::string& name, const GLuint UBO_BP);
		const void addUniform(const std::string& name);
		const GLuint getUniformLocation(const std::string& name);

		const void setUniform1i(const std::string& name, const int value);
		const void setUniform1f(const std::string& name, const float value);
		const void setUniform2f(const std::string& name, const Vec2& vec);
		const void setUniform3f(const std::string& name, const Vec3& vec);
		const void setUniform4f(const std::string& name, const Vec4& vec);
		const void setUniformMat4(const std::string& name, const Mat4& mat);

	private:
		GLuint programID, VertexShaderId, FragmentShaderId;
		std::unordered_map<std::string, GLuint> m_UniformLocation;

		const std::string loadShaderFile(const std::string& shaderPath);
	};
};
