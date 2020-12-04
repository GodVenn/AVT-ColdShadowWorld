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

		// Fields
		unsigned int programId() const { return _programId; }
		std::vector<std::string> uniforms() const { return _uniforms; };

		// Methods
		void bind();
		void unbind();

		void addShader(const std::string& filepath, GLenum shaderType);
		void addAttribute(const GLuint index, const std::string& name);
		void addUniform(const std::string& name);
		void addUniformBlock(const std::string& name, const GLuint UBO_BP);

		GLuint getUniformLocation(const std::string& name);
		void create();

		void setUniform1i(const std::string& name, const int value);
		void setUniform1f(const std::string& name, const float value);
		void setUniform2f(const std::string& name, const Vec2& vec);
		void setUniform3f(const std::string& name, const Vec3& vec);
		void setUniform4f(const std::string& name, const Vec4& vec);
		void setUniformMat4(const std::string& name, const Mat4& mat);

	private:
		GLuint _programId;

		std::unordered_map<std::string, GLuint> _uniformBlocks;
		std::vector<std::string> _uniforms;
		std::unordered_map<std::string, GLuint> _attributes;
		std::unordered_map<std::string, GLenum> _shaders;

		GLuint loadShader(const std::string& filepath, GLenum shaderType);
		std::string parseShader(const std::string& filepath);
		GLint checkCompilation(GLuint shader_id);
		void checkLinkageError();

	};
};
