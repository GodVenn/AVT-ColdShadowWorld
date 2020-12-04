#include "../Headers/pch.h"
#include "../Headers/ShaderProgram.h"


#include <fstream>

namespace engine 
{
	ShaderProgram::ShaderProgram()
		: VertexShaderId(0), FragmentShaderId(0)
	{
		programID = glCreateProgram();
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(programID);
	}

	const void ShaderProgram::load(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// Load vertex shader from the file
		std::string vertexShader = loadShaderFile(vertexSrc);

		// Create an empty vertex shader handle
		VertexShaderId = glCreateShader(GL_VERTEX_SHADER);

		// Send the shader code to GL
		const GLchar* source = (const GLchar*)vertexShader.c_str();
		glShaderSource(VertexShaderId, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(VertexShaderId);

		GLint isCompiled = 0;
		glGetShaderiv(VertexShaderId, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(VertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(VertexShaderId, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			std::cerr << "Vertex shader compilation failure." << std::endl;

			// Don't leak the shader.
			glDeleteShader(VertexShaderId);

			// Exit with failure.
			return;
		}
		// Shader compilation is successful.

		// Load fragment shader from the file
		std::string fragmentShader = loadShaderFile(fragmentSrc);

		// Create an empty fragment shader handle
		FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the shader code to GL
		source = (const GLchar*)fragmentShader.c_str();
		glShaderSource(FragmentShaderId, 1, &source, 0);

		// Compile the Fragment shader
		glCompileShader(FragmentShaderId);

		glGetShaderiv(VertexShaderId, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(FragmentShaderId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(FragmentShaderId, maxLength, &maxLength, &errorLog[0]);

			std::cerr << "Fragment shader compilation failure." << std::endl;

			glDeleteShader(FragmentShaderId);
			glDeleteShader(VertexShaderId);

			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		programID = glCreateProgram();

		glAttachShader(programID, VertexShaderId);
		glAttachShader(programID, FragmentShaderId);
	}

	const void ShaderProgram::link()
	{
		// Link our program
		glLinkProgram(programID);


		GLint isLinked = 0;
		glGetProgramiv(programID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(programID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(programID);
			// Don't leak shaders either.
			glDeleteShader(VertexShaderId);
			glDeleteShader(FragmentShaderId);

			std::cerr << "Shader link failure." << std::endl;

			return;
		}

		
	}

	const void ShaderProgram::create()
	{
		glDetachShader(programID, VertexShaderId);
		glDetachShader(programID, FragmentShaderId);

		glDeleteShader(FragmentShaderId);
		glDeleteShader(VertexShaderId);
	}

	const void ShaderProgram::bind()
	{
		glUseProgram(programID);
	}

	const void ShaderProgram::unbind()
	{
		glUseProgram(0);
	}

	const void ShaderProgram::addAttribute(const GLuint index, const std::string& name)
	{
		glBindAttribLocation(programID, index, name.c_str());	
	}

	const void ShaderProgram::addUniformBlock(const std::string& name, const GLuint UBO_BP)
	{
		
		GLuint UboId = glGetUniformBlockIndex(programID, name.c_str());
		glUniformBlockBinding(programID, UboId, UBO_BP);
	}

	const void ShaderProgram::addUniform(const std::string& name)
	{
		GLuint location = glGetUniformLocation(programID, name.c_str());
		if (location == -1)
		{
			std::cerr << "Warning! Uniform " << name << " doesn't exits" << std::endl;
		}
		// Add the new location into the map.
		m_UniformLocation[name] = location;
	}

	const GLuint ShaderProgram::getUniformLocation(const std::string& name)
	{
		if (m_UniformLocation.find(name) != m_UniformLocation.end()) 
		{
			return m_UniformLocation[name];
		}

		GLuint location = glGetUniformLocation(programID, name.c_str());
		if (location == -1) 
		{
			std::cerr << "Warning! Uniform " << name << " doesn't exits" << std::endl;
		}
		// Add the new location into the map.
		m_UniformLocation[name] = location;

		return location;
	}

	const void ShaderProgram::setUniform1i(const std::string& name, const int value)
	{
		glUniform1i(getUniformLocation(name), value);
	}

	const void ShaderProgram::setUniform1f(const std::string& name, const float value)
	{
		glUniform1f(getUniformLocation(name), value);
	}

	const void ShaderProgram::setUniform2f(const std::string& name, const Vec2& vec)
	{
		glUniform2f(getUniformLocation(name), vec.x, vec.y);
	}

	const void ShaderProgram::setUniform3f(const std::string& name, const Vec3& vec)
	{
		glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z);
	}

	const void ShaderProgram::setUniform4f(const std::string& name, const Vec4& vec)
	{
		glUniform4f(getUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
	}

	const void ShaderProgram::setUniformMat4(const std::string& name, const Mat4& m)
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, m.data);
	}

	const std::string ShaderProgram::loadShaderFile(const std::string& shaderFile)
	{
		std::string f_shader;
		std::string line;

		std::ifstream is_vs("Shaders\\" + shaderFile);

		if (is_vs.is_open()) 
		{
			while (std::getline(is_vs, line))
			{
				f_shader += line + "\n";
			}
			is_vs.close();
		}
		else 
		{
			std::cerr << "Error: Shader file '" << shaderFile << "'could not be found." << std::endl;
		}
		
		return f_shader;

	}
}
