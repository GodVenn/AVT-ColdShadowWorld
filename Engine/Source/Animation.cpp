#include "../Headers/pch.h"
#include "../Headers/Animation.h"

namespace engine
{

	Animation::Animation(State state, SceneNode* node, float duration)
	{
		this->state = state;
		this->duration = duration;
		this->currTime = 0.f;
		this->currAnimState = AnimationState::Animating;
		this->node = node;
	}

	Animation::~Animation()
	{
	}

	void Animation::updateAnim(float deltaTime)
	{
		currTime += deltaTime;
		
		if (currAnimState != AnimationState::Stopped)
		{
			if (currTime >= duration)
			{
				this->currAnimState = AnimationState::Stopped;
				this->currTime = 0.f;				
				this->state.currPos = this->state.posFinal;
				node->setMatrix(this->state.posFinal);
				return;
			}
			else
			{					
				
				this->state.currPos = state.posIni + (currTime / duration) * (state.posFinal - state.posIni);
				node->setMatrix(this->state.currPos);
				
			}
		}	

	}

	void Animation::reset()
	{
		this->currTime = 0.f;
		this->currAnimState = AnimationState::Animating;
		node->setMatrix(this->state.posIni);
	}

	State::State(Mat4& posIni, Mat4& posFinal)
	{
		this->posIni = posIni;
		this->posFinal = posFinal;
	}

	

	

}