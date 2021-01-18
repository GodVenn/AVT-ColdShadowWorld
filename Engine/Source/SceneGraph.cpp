#include "../Headers/pch.h"
#include "../Headers/SceneGraph.h"


namespace engine
{
	static GLuint VboId;
	static int UBO_CAMERA = 0;
	SceneGraph::SceneGraph()
	{
		this->root = new SceneNode();
		this->camera = nullptr;
		
		// TODO TEMPORARY SORT THIS OUT
		/// CREATE UNIFORM BUFFER
		/**/
		glGenBuffers(1, &VboId);
		glBindBuffer(GL_UNIFORM_BUFFER, VboId);
		{

			glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 16 * 2, 0, GL_STREAM_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, UBO_CAMERA, VboId);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		/**/
	}

	SceneGraph::~SceneGraph()
	{

		delete this->root;

		// TODO TEMPORARY SORT THIS OUT
		glDeleteBuffers(1, &VboId);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
		//this->camera->draw();
		// TODO: TEMPORARY SORT THIS OUT
		// IN ORDER TO HAVE ONE SCENE WITH MULTIPLE CAMERAS
		/**/
		glBindBuffer(GL_UNIFORM_BUFFER, VboId);
		{
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat) * 16, camera->getViewMatrix().data);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 16, sizeof(GLfloat) * 16, camera->getProjMatrix().data);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		/**/
		// Draw scene
		this->root->draw();
	}

}