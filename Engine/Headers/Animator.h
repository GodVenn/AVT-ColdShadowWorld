#pragma once

#include "Animation.h"

namespace engine 
{
	class Animator
	{
	public:
		std::vector<Animation> animations;
		Animator();
		~Animator();

		void addAnimation(Animation anim);
		void playAnimation(float deltaTime);
		void playAnimationLoop(float deltaTime);
		void playAllAnimation(float deltaTime);
		void playAllAnimationLoop(float deltaTime);
		void toggle();
		void stopAnimation();
		void resetAnimation();
		bool isFinish();

	private:
		size_t currAnim;
		bool isActive;
	};
}