#pragma once
#include "Camera.h"
#include "Vector.h"
#include "Constants.h"

namespace engine 
{
	class CameraController {
	public:
		float sensitivity = 0.1f;
		float movementSpeed = 2.0f;
		Camera* cam;

		CameraController();
		CameraController(Camera& camToMove, const float windowHeight, const float windowWidth, const float initialYaw = -90, const float initialPitch = 0);

		/// <summary>
		/// Update the movement parameters for the camera angle
		/// </summary>
		/// <param name="newX"> - xPosition from the mouse position callback </param>
		/// <param name="newY"> - yPosition from the mouse position callback </param>
		void updateYawPitch(const float newX, const float newY);

		/// <summary>
		/// Update the movement parameters for the camera position based on booleans for each direction.
		/// </summary>
		/// <param name="right"></param>
		/// <param name="left"></param>
		/// <param name="forward"></param>
		/// <param name="backwards"></param>
		/// <param name="elapsed_time"></param>
		void updateMovement(const int right, const int left, const int forward, const int backwards, const float elapsed_time);

		/// <summary>
		/// Updates the camera according to the movement parameters.
		/// This is where the actual movement is implemented.
		/// </summary>
		void moveCam();
	private:
		Vec3 _cameraMovement;
		float _lastX;
		float _lastY;
		bool _firstMouse;
		float _yaw;
		float _pitch;
	};
}