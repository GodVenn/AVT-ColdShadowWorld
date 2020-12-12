#pragma once
#include "Singleton.h"
#include "IManager.h"
#include "Animation.h"

namespace engine 
{
	class AnimatorManager : public Singleton<AnimatorManager>, public IManager<Animation>
	{
		friend Singleton<AnimatorManager>;

		AnimatorManager();
		~AnimatorManager();

	public:
		virtual void remove(std::string animatorName) override;
		virtual Animation* get(std::string animatorName) override;
		virtual void add(std::string animatorName, Animation* animator) override;
	};


}