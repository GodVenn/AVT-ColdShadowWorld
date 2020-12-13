#pragma once
#include "../Headers/Vector.h"

namespace engine
{
	class Input
	{
	public:
		static bool isKeyPressed(const int keyCode);
		static bool isMouseButtonPressed(const int button);
		static Vec2 getMousePosition();
		static float getMouseX();
		static float getMouseY();

	};
}