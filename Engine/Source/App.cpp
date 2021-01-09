#include "../Headers/pch.h"
#include "../Headers/App.h"
#include "../Headers/gl.h"

namespace engine 
{
	App::App()
	{
		this->deltaTime = 0;
		this->minor = this->major = 0;
		this->windowWidth = this->windowHeight = 0;
		this->fullScreen = this->vsync = 0;
		this->glApp = nullptr;
		this->window = nullptr;
		this->windowsTittle = NULL;
	}
	App::~App()
	{
		if (glApp)
		{
			delete glApp;
		}
	}
	/////////////////////////////////////////////////////////////////////// SETUP
	void App::init()
	{
		setupGLFW();
		setupGLEW();
		setupOpenGL();

		//setupErrorCallback();
		glApp->initApp();
	}
	/////////////////////////////////////////////////////////////////////// RUN
	void App::run()
	{
		double last_time = getTime();
		while (!glfwWindowShouldClose(this->window))
		{
			double time = getTime();
			deltaTime = static_cast<float>(time - last_time);
			last_time = time;

			// Double Buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glApp->displayCallback(this->window, deltaTime);
			glfwSwapBuffers(this->window);
			glfwPollEvents();
		}
		glfwDestroyWindow(this->window);
		glfwTerminate();
	}

	/////////////////////////////////////////////////////////////////////// GETTERs AND SETTERs
	IApp* App::getGLApp()
	{
		return this->glApp;
	}

	void App::setGLApp(IApp* glApp)
	{
		this->glApp = glApp;
	}

	void App::setupOpenGL(int major, int minor)
	{
		this->major = major;
		this->minor = minor;
	}

	void App::setupWindow(int width, int height, const char* title, int fullScreen, int vsync)
	{
		this->windowHeight = height;
		this->windowWidth = width;
		this->windowsTittle = title;
		this->fullScreen = fullScreen;
		this->vsync = vsync;
	}
	/////////////////////////////////////////////////////////////////////// GLWF
	void glfw_error_callback(int error, const char* description)
	{
		std::cerr << "GLFW Error: " << description << std::endl;
	}
	void App::setupGLFW()
	{
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
		{
			exit(EXIT_FAILURE);
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->minor);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

		setupWindow();
		setupCallbacks();

	#if _DEBUG
			std::cout << "GLFW " << glfwGetVersionString() << std::endl;
	#endif
	}

	void App::setupWindow()
	{
		GLFWmonitor* monitor = this->fullScreen ? glfwGetPrimaryMonitor() : 0;
		this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, this->windowsTittle, monitor, 0);
		if (!window)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(window);
		glfwSwapInterval(this->vsync);
	
	}

	/////////////////////////////////////////////////////////////////////// GLEM
	void App::setupGLEW()
	{
		glewExperimental = GL_TRUE;
		// Allow extension entry points to be loaded even if the extension isn't 
		// present in the driver's extensions string.
		GLenum result = glewInit();
		if (result != GLEW_OK)
		{
			std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
			exit(EXIT_FAILURE);
		}
		GLenum err_code = glGetError();
		// You might get GL_INVALID_ENUM when loading GLEW.
	}

	/////////////////////////////////////////////////////////////////////// OPENGL
	void App::setupOpenGL()
	{
	#if _DEBUG
			checkOpenGLInfo();
	#endif
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		glDepthRange(0.0, 1.0);
		glClearDepth(1.0);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glViewport(0, 0, this->windowWidth, this->windowHeight);
	}

	/////////////////////////////////////////////////////////////////////// CALLBACKs WHAPPERs
	static void window_close_callback_wrapper(GLFWwindow* win)
	{
		IApp* app = (IApp*)glfwGetWindowUserPointer(win);
		app->window_close_callback(win);
	}

	static void window_size_callback_wrapper(GLFWwindow* win, int winx, int winy)
	{
		IApp* app = (IApp*)glfwGetWindowUserPointer(win);
		app->window_size_callback(win, winx, winy);
	}


	static void key_callback_wrapper(GLFWwindow* win, int key, int scancode, int action, int mods)
	{
		IApp* app = (IApp*)glfwGetWindowUserPointer(win);
		app->key_callback(win, key, scancode, action, mods);
	}

	static void mouse_callback_wrapper(GLFWwindow* win, double xpos, double ypos)
	{
		IApp* app = (IApp*)glfwGetWindowUserPointer(win);
		app->mouse_callback(win, xpos, ypos);
	}
	static void scroll_callback_wrapper(GLFWwindow* win, double xoffset, double yoffset)
	{
		IApp* app = (IApp*)glfwGetWindowUserPointer(win);
		app->scroll_callback(win, xoffset, yoffset);
	}

	/////////////////////////////////////////////////////////////////////// CALLBACKs
	void App::setupCallbacks()
	{
		glfwSetWindowUserPointer(this->window, glApp);
		glfwSetWindowCloseCallback(this->window, window_close_callback_wrapper);
		glfwSetWindowSizeCallback(this->window, window_size_callback_wrapper);
		glfwSetKeyCallback(this->window, key_callback_wrapper);
		glfwSetCursorPosCallback(this->window, mouse_callback_wrapper);
		glfwSetScrollCallback(this->window, scroll_callback_wrapper);
	}



}