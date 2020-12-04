#pragma once
#include "Singleton.h"
#include "IManager.h"
#include "Animator.h"

namespace engine 
{
	class AnimatorManager : public Singleton<AnimatorManager>, public IManager<Animator>
	{
		friend Singleton<AnimatorManager>;

		AnimatorManager();
		~AnimatorManager();

	public:
		virtual void remove(std::string animatorName) override;
		virtual Animator* get(std::string animatorName) override;
		virtual void add(std::string animatorName, Animator* animator) override;
	};


}