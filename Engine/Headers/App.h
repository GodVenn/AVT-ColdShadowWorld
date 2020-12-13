#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Singleton.h"

namespace engine
{
	//////////////////////////////////////////////////////////////////////
	class IApp;
	class App;

	////////////////////////////////////////////////////////////////////// IApp
	class IApp
	{
	public:
		virtual void initApp() = 0;
		virtual void displayCallback(GLFWwindow* win, float deltaTime) = 0;
		virtual void window_close_callback(GLFWwindow* win) = 0;
		virtual void window_size_callback(GLFWwindow* win, int winx, int winy) = 0;
		virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) = 0;
		virtual void mouse_callback(GLFWwindow* win, double xpos, double ypos) = 0;
		virtual void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) = 0;
	};

	////////////////////////////////////////////////////////////////////// App
	class App : public Singleton<App>
	{
	public:
		int windowWidth, windowHeight;
		float deltaTime;
		IApp* getGLApp();
		void setGLApp(IApp* glApp);
		void setupOpenGL(int major, int minor);
		void setupWindow(int width, int height, const char* title, int fullScreen, int vsync);

		inline GLFWwindow* getWindow() { return window; }
		void init();
		void run();

	private:
		friend Singleton<App>;
		App();
		~App();
		IApp* glApp;
		const char* windowsTittle;
		int fullScreen;
		int vsync;
		int minor, major;
		GLFWwindow* window;


		void setupWindow();
		void setupOpenGL();
		void setupGLEW();
		void setupGLFW();

		void setupCallbacks();


	};
}