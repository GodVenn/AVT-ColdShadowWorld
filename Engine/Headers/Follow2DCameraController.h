#pragma once
#include "Camera.h"
#include "CameraController.h"
namespace engine
{
	class Follow2DCameraController 
	{

	public:
		Follow2DCameraController(Camera* camera, CameraController* followController, float verticalOffset = 50.0f);
		~Follow2DCameraController();

		void update(float deltaTime);
		void setVerticalOffset(const float offset);
	private:
		Camera* _camera;
		CameraController* _followController;
		float _verticalOffset;
	
	};
}