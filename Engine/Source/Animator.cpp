#include "../Headers/pch.h"
#include "../Headers/Animator.h"

namespace engine 
{

	Animator::Animator()
	{
		this->isActive = false;
		this->currAnim = 0;
	}

	Animator::~Animator()
	{
		animations.clear();
	}

	void Animator::addAnimation(Animation anim)
	{
		this->animations.push_back(anim);
	}

	void Animator::playAnimation(float deltaTime)
	{
		if (this->isActive)
		{
			if (animations[currAnim].currAnimState == AnimationState::Animating)
			{
				animations[currAnim].updateAnim(deltaTime);
			}
			else
			{
				if (currAnim < animations.size() - (size_t)1)
				{
					currAnim++;
				}
				else
				{
					currAnim = animations.size() - (size_t)1;

				}
			}
		}

	}

	void Animator::playAnimationLoop(float deltaTime)
	{
		if (isFinish())
		{
			resetAnimation();
		}
		playAnimation(deltaTime);
	}

	void Animator::playAllAnimation(float deltaTime)
	{
		if (this->isActive)
		{
			for (auto& anim : animations)
			{
				anim.updateAnim(deltaTime);
			}
		}
	}

	void Animator::playAllAnimationLoop(float deltaTime)
	{
		if (isFinish())
		{
			resetAnimation();
		}
		playAllAnimation(deltaTime);
	}

	void Animator::toggle()
	{
		this->isActive = true;
	}

	void Animator::stopAnimation()
	{
		this->isActive = false;
	}

	void Animator::resetAnimation()
	{
		int numClimps = static_cast<int>(animations.size()) - 1;
		for(int i = numClimps; i >= 0; i--)
		{
			animations[i].reset();
			
		}
		currAnim = 0;
	}

	bool Animator::isFinish()
	{
		for (int i = 0; i < animations.size(); i++)
		{
			if (animations[i].currAnimState == AnimationState::Animating)
			{
				return false;
			}

		}
		return true;
	}
}