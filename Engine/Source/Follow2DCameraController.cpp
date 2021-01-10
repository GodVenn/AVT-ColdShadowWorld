#include "../Headers/pch.h"
#include "../Headers/Follow2DCameraController.h"

namespace engine
{
	Follow2DCameraController::Follow2DCameraController(Camera* camera, CameraController* followController, float verticalOffset)
		: _camera(camera), _followController(followController), _verticalOffset(verticalOffset)
	{
	}

	Follow2DCameraController::~Follow2DCameraController()
	{
	}

	void Follow2DCameraController::update(float deltaTime)
	{
		//Update camera position
		auto pos = _followController->cam->eye;
		_camera->eye = { pos.x, pos.y + _verticalOffset, pos.z };
		//Update looking direction
		_camera->center = pos;

	}
}