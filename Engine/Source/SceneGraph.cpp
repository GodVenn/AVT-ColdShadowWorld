#include "../Headers/pch.h"
#include "../Headers/SceneGraph.h"

namespace engine
{
	SceneGraph::SceneGraph()
	{
		this->root = new SceneNode();
		this->camera = nullptr;
	}

	SceneGraph::~SceneGraph()
	{
		delete this->root;
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