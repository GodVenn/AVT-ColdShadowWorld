#pragma once
#include "Quaternion.h"
#include "SceneNode.h"

namespace engine
{
	///////////////////////////////////////////////////////////////////////
	enum class AnimationState;
	struct State;
	class Animation;

	/////////////////////////////////////////////////////////////////////// AnimationState
	enum class AnimationState {
		Stopped,  Animating
	};

	/////////////////////////////////////////////////////////////////////// State
	struct State 
	{
		State() = default;
		State(Mat4& posIni, Mat4& posFinal);
		
		Mat4 posIni;
		Mat4 posFinal;
		Mat4 currPos;

	};

	/////////////////////////////////////////////////////////////////////// Animation
	class Animation
	{
	public:
		State state;
		SceneNode* node;
		AnimationState currAnimState;

		Animation(State state, SceneNode* node, float duration);
		~Animation();

		void updateAnim(float deltaTime);
		void reset();

	private:
		float currTime;
		float duration;
	};



}