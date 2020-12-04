#include "../Headers/pch.h"
#include "../Headers/SceneGraph.h"

namespace engine
{
	SceneGraph::SceneGraph()
	{
		this->root = new SceneNode();
		this->camera = new Camera();
	}

	SceneGraph::~SceneGraph()
	{
		delete this->root;
		delete this->camera;
	}

	SceneNode* SceneGraph::createNode()
	{
		return root->createNode();
	}

	SceneNode* SceneGraph::getRoot()
	{
		return this->root;
	}

	void SceneGraph::setCamera(Camera* cam)
	{
		this->camera = cam;
	}

	Camera* SceneGraph::getCamera()
	{
		return this->camera;
	}

	void SceneGraph::draw()
	{
		/// CAMERA
		this->camera->draw();
		this->root->draw();
	}

}