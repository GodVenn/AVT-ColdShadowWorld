#pragma once
#include "SceneNode.h"

namespace engine
{
	/////////////////////////////////////////////////////////////////////// BackMode
	class BackMode : public ISceneNodeCallBack
	{
	public:
		BackMode() {};
		~BackMode() {};

		virtual void beforeDraw(SceneNode* node) override
		{
			glCullFace(GL_FRONT);// Removing front faces
		   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		virtual void afterDraw(SceneNode* node) override
		{
			glCullFace(GL_BACK); // Removing back faces
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //DRAW WIREFRAME
		}

	};
	/////////////////////////////////////////////////////////////////////// DisableDepthCallback
	class DisableDepthCallback : public ISceneNodeCallBack
	{
	public:
		DisableDepthCallback() {};
		~DisableDepthCallback() {};

		virtual void beforeDraw(SceneNode* node) override
		{
			glDisable(GL_DEPTH_TEST);
		}
		virtual void afterDraw(SceneNode* node) override 
		{
			glEnable(GL_DEPTH_TEST);
		}

	};


	/////////////////////////////////////////////////////////////////////// WireframeCallback
	class WireframeCallback : public ISceneNodeCallBack
	{
	public:
		WireframeCallback() {};
		~WireframeCallback() {};

		virtual void beforeDraw(SceneNode* node) override
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		virtual void afterDraw(SceneNode* node) override 
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

	};
}