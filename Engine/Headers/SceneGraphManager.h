#pragma once

#include "Singleton.h"
#include "IManager.h"
#include "SceneGraph.h"

namespace engine
{
	class SceneGraphManager : public Singleton<SceneGraphManager>, public IManager<SceneGraph>
	{
		friend Singleton<SceneGraphManager>;
		SceneGraphManager();
		~SceneGraphManager();
	public:

		virtual void remove(std::string sceneName) override;
		virtual SceneGraph* get(std::string sceneName) override;
		virtual void add(std::string sceneName, SceneGraph* scene) override;

	};


}