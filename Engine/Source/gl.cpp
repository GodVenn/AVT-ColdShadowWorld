#include "../Headers/pch.h"
#include "../Headers/gl.h"


namespace engine
{
	void checkOpenGLInfo()
	{
		const GLubyte* renderer = glGetString(GL_RENDERER);
		const GLubyte* vendor = glGetString(GL_VENDOR);
		const GLubyte* version = glGetString(GL_VERSION);
		const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
		std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
		std::cerr << "OpenGL version " << version << std::endl;
		std::cerr << "GLSL version " << glslVersion << std::endl;
	}

	double getTime()
	{
		return glfwGetTime();
	}

	static const std::string errorSource(GLenum source)
	{
		switch (source) {
		case GL_DEBUG_SOURCE_API:				return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
		case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
		case GL_DEBUG_SOURCE_APPLICATION:		return "application";
		case GL_DEBUG_SOURCE_OTHER:				return "other";
		default:								exit(EXIT_FAILURE);
		}
	}

	static const std::string errorType(GLenum type)
	{
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:				return "error";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
		case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
		case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
		case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
		case GL_DEBUG_TYPE_PUSH_GROUP:			return "push group";
		case GL_DEBUG_TYPE_POP_GROUP:			return "pop group";
		case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
		default:								exit(EXIT_FAILURE);
		}
	}

	static const std::string errorSeverity(GLenum severity)
	{
		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:			return "high";
		case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
		case GL_DEBUG_SEVERITY_LOW:				return "low";
		case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
		default:								exit(EXIT_FAILURE);
		}
	}
	static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam)
	{
		std::cerr << "GL ERROR:" << std::endl;
		std::cerr << "  source:     " << errorSource(source) << std::endl;
		std::cerr << "  type:       " << errorType(type) << std::endl;
		std::cerr << "  severity:   " << errorSeverity(severity) << std::endl;
		std::cerr << "  debug call: " << std::endl << message << std::endl;
		std::cerr << "Press <return>.";
		std::cin.ignore();
	}

	void setupErrorCallback()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(error, 0);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);
		// params: source, type, severity, count, ids, enabled
	}
}