#include "../Headers/pch.h"
#include "../Headers/ShaderProgram.h"


#include <fstream>

namespace engine 
{

	ShaderProgram::ShaderProgram() {
		this->_programId = 0;
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(_programId);
	}

	void ShaderProgram::bind()
	{
		glUseProgram(this->_programId);
	}

	void ShaderProgram::unbind()
	{
		glUseProgram(0);
	}

	void ShaderProgram::addShader(const std::string& filepath, GLenum shaderType)
	{
		this->_shaders.insert(std::make_pair(filepath, shaderType));
	}

	void ShaderProgram::addAttribute(const GLuint attribPos, const std::string& attribName)
	{
		this->_attributes.insert(std::make_pair(attribName, attribPos));
	}

	void ShaderProgram::addUniformBlock(const std::string& name, const GLuint UBO_BP)
	{	
		this->_uniformBlocks.insert(std::make_pair(name, UBO_BP));
	}

	void ShaderProgram::addUniform(const std::string& name)
	{
		this->_uniforms.push_back(name);
	}

	GLuint ShaderProgram::getUniformLocation(const std::string& name)
	{
		GLuint location = glGetUniformLocation(_programId, name.c_str());
		if (location == -1)
		{
			std::cerr << "Warning! Uniform " << name << " doesn't exist" << std::endl;
		}
		return location;
	}

	void ShaderProgram::create()
	{
		std::vector<GLuint> shaderIds;
		this->_programId = glCreateProgram();

		// Load Shaders
		for (std::pair<std::string, GLenum> shader : this->_shaders)
		{
			shaderIds.push_back(this->loadShader(shader.first, shader.second));
		}

		// Bind Attributes
		for (std::pair<std::string, GLuint> attribute : this->_attributes)
		{
			glBindAttribLocation(this->_programId, attribute.second, attribute.first.c_str());
		}

		glLinkProgram(_programId);
		this->checkLinkageError();

		// Bind Uniform Blocks
		for (std::pair<std::string, GLuint> block : this->_uniformBlocks)
		{
			GLuint UboId = glGetUniformBlockIndex(_programId, block.first.c_str());
			if (UboId == -1)
			{
				std::cerr << "Warning! Uniform block " << block.first << " doesn't exist" << std::endl;
			}
			glUniformBlockBinding(_programId, UboId, block.second);
		}

		for (GLuint id : shaderIds)
		{
			glDetachShader(this->_programId, id);
			glDeleteShader(id);
		}
		this->_shaders.clear();
		this->_attributes.clear();
	}

	void ShaderProgram::setUniform1i(const std::string& name, const int value)
	{
		glUniform1i(getUniformLocation(name), value);
	}

	void ShaderProgram::setUniform1f(const std::string& name, const float value)
	{
		glUniform1f(getUniformLocation(name), value);
	}

	void ShaderProgram::setUniform2f(const std::string& name, const Vec2& vec)
	{
		glUniform2f(getUniformLocation(name), vec.x, vec.y);
	}

	void ShaderProgram::setUniform3f(const std::string& name, const Vec3& vec)
	{
		glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z);
	}

	void ShaderProgram::setUniform4f(const std::string& name, const Vec4& vec)
	{
		glUniform4f(getUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
	}

	void ShaderProgram::setUniformMat4(const std::string& name, const Mat4& m)
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, m.data);
	}

	GLuint ShaderProgram::loadShader(const std::string& filepath, GLenum shaderType) {
		GLuint shaderId = glCreateShader(shaderType);
		std::string tempStr = this->parseShader(filepath);
		GLchar* shaderContents = (GLchar*)tempStr.c_str();
		glShaderSource(shaderId, 1, &shaderContents, 0);
		glCompileShader(shaderId);
		this->checkCompilation(shaderId);
		glAttachShader(_programId, shaderId);
		return shaderId;
	}

	std::string ShaderProgram::parseShader(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::string line;
		std::string result;
		if (stream.is_open()) {
			while (std::getline(stream, line)) {
				result += line + "\n";
			}
		}
		else std::cout << "Error: Shader file '" << filepath << "'could not be found." << std::endl;

		return result;
	}

	GLint ShaderProgram::checkCompilation(GLuint shader_id) {
		GLint result;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

			GLchar* strInfoLog = new GLchar[length];// + 1 if fails
			glGetShaderInfoLog(shader_id, length, &length, strInfoLog);

			fprintf(stderr, "Compilation error in shader: %s\n", strInfoLog);
			delete[] strInfoLog;
			exit(EXIT_FAILURE);
		}
		return result;
	}

	void ShaderProgram::checkLinkageError()
	{
		GLint linked;
		glGetProgramiv(this->_programId, GL_LINK_STATUS, &linked);
		if (linked == GL_FALSE)
		{
			GLint length;
			glGetProgramiv(this->_programId, GL_INFO_LOG_LENGTH, &length);
			GLchar* const log = new char[length];
			glGetProgramInfoLog(this->_programId, length, &length, log);
			std::cerr << "[LINK] " << std::endl << log << std::endl;
			delete[] log;
			exit(EXIT_FAILURE);
		}
	}

	/// ////////////////////////////////////////////////////////////////////////////////////////////
	///// </summary>
	///// <param name="vertexSrc"></param>
	///// <param name="fragmentSrc"></param>
	///// <returns></returns>
	//const void ShaderProgram::load(const std::string& vertexSrc, const std::string& fragmentSrc)
	//{
	//	// Load vertex shader from the file
	//	std::string vertexShader = loadShaderFile(vertexSrc);

	//	// Create an empty vertex shader handle
	//	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);

	//	// Send the shader code to GL
	//	const GLchar* source = (const GLchar*)vertexShader.c_str();
	//	glShaderSource(VertexShaderId, 1, &source, 0);

	//	// Compile the vertex shader
	//	glCompileShader(VertexShaderId);

	//	GLint isCompiled = 0;
	//	glGetShaderiv(VertexShaderId, GL_COMPILE_STATUS, &isCompiled);
	//	if (isCompiled == GL_FALSE)
	//	{
	//		GLint maxLength = 0;
	//		glGetShaderiv(VertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);

	//		// The maxLength includes the NULL character
	//		std::vector<GLchar> errorLog(maxLength);
	//		glGetShaderInfoLog(VertexShaderId, maxLength, &maxLength, &errorLog[0]);

	//		// Provide the infolog in whatever manor you deem best.
	//		std::cerr << "Vertex shader compilation failure." << std::endl;

	//		// Don't leak the shader.
	//		glDeleteShader(VertexShaderId);

	//		// Exit with failure.
	//		return;
	//	}
	//	// Shader compilation is successful.

	//	// Load fragment shader from the file
	//	std::string fragmentShader = loadShaderFile(fragmentSrc);

	//	// Create an empty fragment shader handle
	//	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	//	// Send the shader code to GL
	//	source = (const GLchar*)fragmentShader.c_str();
	//	glShaderSource(FragmentShaderId, 1, &source, 0);

	//	// Compile the Fragment shader
	//	glCompileShader(FragmentShaderId);

	//	glGetShaderiv(VertexShaderId, GL_COMPILE_STATUS, &isCompiled);
	//	if (isCompiled == GL_FALSE)
	//	{
	//		GLint maxLength = 0;
	//		glGetShaderiv(FragmentShaderId, GL_INFO_LOG_LENGTH, &maxLength);

	//		// The maxLength includes the NULL character
	//		std::vector<GLchar> errorLog(maxLength);
	//		glGetShaderInfoLog(FragmentShaderId, maxLength, &maxLength, &errorLog[0]);

	//		std::cerr << "Fragment shader compilation failure." << std::endl;

	//		glDeleteShader(FragmentShaderId);
	//		glDeleteShader(VertexShaderId);

	//		return;
	//	}

	//	// Vertex and fragment shaders are successfully compiled.
	//	programID = glCreateProgram();

	//	glAttachShader(programID, VertexShaderId);
	//	glAttachShader(programID, FragmentShaderId);
	//}

	//const void ShaderProgram::link()
	//{
	//	// Link our program
	//	glLinkProgram(programID);


	//	GLint isLinked = 0;
	//	glGetProgramiv(programID, GL_LINK_STATUS, (int*)&isLinked);
	//	if (isLinked == GL_FALSE)
	//	{
	//		GLint maxLength = 0;
	//		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

	//		// The maxLength includes the NULL character
	//		std::vector<GLchar> infoLog(maxLength);
	//		glGetProgramInfoLog(programID, maxLength, &maxLength, &infoLog[0]);

	//		// We don't need the program anymore.
	//		glDeleteProgram(programID);
	//		// Don't leak shaders either.
	//		glDeleteShader(VertexShaderId);
	//		glDeleteShader(FragmentShaderId);

	//		std::cerr << "Shader link failure." << std::endl;

	//		return;
	//	}

	//	
	//}

	//const void ShaderProgram::create()
	//{
	//	glDetachShader(programID, VertexShaderId);
	//	glDetachShader(programID, FragmentShaderId);

	//	glDeleteShader(FragmentShaderId);
	//	glDeleteShader(VertexShaderId);
	//}

	//const void ShaderProgram::bind()
	//{
	//	glUseProgram(programID);
	//}

	//const void ShaderProgram::unbind()
	//{
	//	glUseProgram(0);
	//}

	//const void ShaderProgram::addAttribute(const GLuint index, const std::string& name)
	//{
	//	glBindAttribLocation(programID, index, name.c_str());	
	//}

	//const void ShaderProgram::addUniformBlock(const std::string& name, const GLuint UBO_BP)
	//{
	//	
	//	GLuint UboId = glGetUniformBlockIndex(programID, name.c_str());
	//	glUniformBlockBinding(programID, UboId, UBO_BP);
	//}

	//const void ShaderProgram::addUniform(const std::string& name)
	//{
	//	GLuint location = glGetUniformLocation(programID, name.c_str());
	//	if (location == -1)
	//	{
	//		std::cerr << "Warning! Uniform " << name << " doesn't exits" << std::endl;
	//	}
	//	// Add the new location into the map.
	//	m_UniformLocation[name] = location;
	//}

	//const GLuint ShaderProgram::getUniformLocation(const std::string& name)
	//{
	//	if (m_UniformLocation.find(name) != m_UniformLocation.end()) 
	//	{
	//		return m_UniformLocation[name];
	//	}

	//	GLuint location = glGetUniformLocation(programID, name.c_str());
	//	if (location == -1) 
	//	{
	//		std::cerr << "Warning! Uniform " << name << " doesn't exits" << std::endl;
	//	}
	//	// Add the new location into the map.
	//	m_UniformLocation[name] = location;

	//	return location;
	//}


	//const std::string ShaderProgram::loadShaderFile(const std::string& shaderFile)
	//{
	//	std::string f_shader;
	//	std::string line;

	//	std::ifstream is_vs("Shaders\\" + shaderFile);

	//	if (is_vs.is_open()) 
	//	{
	//		while (std::getline(is_vs, line))
	//		{
	//			f_shader += line + "\n";
	//		}
	//		is_vs.close();
	//	}
	//	else 
	//	{
	//		std::cerr << "Error: Shader file '" << shaderFile << "'could not be found." << std::endl;
	//	}
	//	
	//	return f_shader;

	//}
}
