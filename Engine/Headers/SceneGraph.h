#pragma once

#include "SceneNode.h"
#include "Camera.h"

namespace engine
{
	class SceneGraph
	{
	public:
		SceneGraph();
		~SceneGraph();

		SceneNode* createNode();
		SceneNode* getRoot();
		void setCamera(Camera* cam);
		Camera* getCamera();

		void draw();

	private:
		SceneNode* root;
		Camera* camera;
	};
}