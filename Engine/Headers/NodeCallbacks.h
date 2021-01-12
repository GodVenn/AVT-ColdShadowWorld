#pragma once
#include "SceneNode.h"

namespace engine
{
	/////////////////////////////////////////////////////////////////////// DisableDepthCallback
	class FilterPixelCallback : public ISceneNodeCallBack
	{
	public:
		FilterPixelCallback() {};
		~FilterPixelCallback() {};

		virtual void beforeDraw(SceneNode* node) override
		{
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);	// All fragments should pass the stencil test
			glStencilMask(0xFF);				// Able to write in the stencil buffer
		}
		virtual void afterDraw(SceneNode* node) override
		{
			glStencilFunc(GL_EQUAL, 1, 0xFF);
			glStencilMask(0x00);				// disable writing to the stencil buffer
			glDisable(GL_DEPTH_TEST);
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
}