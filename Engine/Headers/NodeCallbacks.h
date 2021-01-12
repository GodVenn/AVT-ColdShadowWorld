#pragma once
#include "SceneNode.h"

namespace engine
{
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