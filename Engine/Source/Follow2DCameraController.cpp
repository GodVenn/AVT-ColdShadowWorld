#include "../Headers/pch.h"
#include "../Headers/Follow2DCameraController.h"
#include "../Headers/Input.h"
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
		_camera->eye = { _followController->cam->eye.x, _verticalOffset,  _followController->cam->eye.z };
		_camera->center = _followController->cam->center;
	}
	void Follow2DCameraController::setVerticalOffset(const float offset)
	{
		_verticalOffset = offset;
	}
}