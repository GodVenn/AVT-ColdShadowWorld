#include "../Headers/pch.h"
#include "../Headers/CameraController.h"

namespace engine {
	CameraController::CameraController()
	{
		this->cam = nullptr;
		_lastX = 0;
		_lastY = 0;
		_firstMouse = true;
		_yaw = 0;
		_pitch = 0;
	}

	CameraController::CameraController(Camera& camToMove, float windowHeight, float windowWidth, float initialYaw, float initialPitch)
	{
		this->cam = &camToMove;
		_lastX = windowWidth / 2;
		_lastY = windowHeight / 2;
		_firstMouse = true;
		_yaw = initialYaw;
		_pitch = initialPitch;
	}

	void CameraController::updateYawPitch(float newX, float newY)
	{
		if (_firstMouse)
		{
			this->_lastX = newX;
			this->_lastY = newY;
			this->_firstMouse = false;
		}

		float xoffset = newX - this->_lastX;
		float yoffset = this->_lastY - newY;
		this->_lastX = newX;
		this->_lastY = newY;

		xoffset *= this->sensitivity;
		yoffset *= this->sensitivity;

		this->_yaw += xoffset;
		this->_pitch += yoffset;

		if (this->_pitch > 89.0f)
			this->_pitch = 89.0f;
		if (this->_pitch < -89.0f)
			this->_pitch = -89.0f;
	}

	void CameraController::updateMovement(const int right, const int left, const int forward, const int backwards, const float elapsed_time)
	{
		const Vec3 view = normalize(cam->center - cam->eye);
		Vec3 forwardMvmt = view * (float)(forward - backwards) * this->movementSpeed * elapsed_time;

		Vec3 sidewaysMvmt = Vec3();
		if (right || left) {
			const Vec3 side = normalize(view.crossProduct(cam->up));
			sidewaysMvmt = side * (float)(right - left) * this->movementSpeed * elapsed_time;
		}

		this->_cameraMovement = sidewaysMvmt + forwardMvmt;

	}

	void CameraController::moveCam()
	{
		if (this->cam == nullptr) {
			std::cerr << "ERROR: No camera attached to the cameraMover" << std::endl;
			exit(EXIT_FAILURE);
		}

		const float yawRad = _yaw * engine::DEGREES_TO_RADIANS;
		const float pitchRad = _pitch * DEGREES_TO_RADIANS;

		Vec3 direction;
		direction.x = cos(yawRad) * cos(pitchRad);
		direction.y = sin(pitchRad);
		direction.z = sin(yawRad) * cos(pitchRad);

		this->cam->center = this->cam->eye + direction;

		this->cam->center += this->_cameraMovement;
		this->cam->eye += this->_cameraMovement;

		this->_cameraMovement = Vec3();
	}
}