#include "../Headers/pch.h"
#include "../Headers/Input.h"
#include "../Headers/App.h"

namespace engine
{
	bool Input::isKeyPressed(const int keyCode)
	{
		auto win = App::getInstance()->getWindow();
		int state = glfwGetKey(win, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::isMouseButtonPressed(const int button)
	{
		auto win = App::getInstance()->getWindow();
		int state = glfwGetMouseButton(win, button);
		return state == GLFW_PRESS;
	}
	Vec2 Input::getMousePosition()
	{
		auto win = App::getInstance()->getWindow();
		double xpos, ypos;
		glfwGetCursorPos(win, &xpos, &ypos);
		return Vec2(static_cast<float>(xpos), static_cast<float>(ypos));
	}
	float Input::getMouseX()
	{
		return getMousePosition().x;
	}
	float Input::getMouseY()
	{
		return getMousePosition().y;
	}
}