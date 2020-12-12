#include "../Headers/pch.h"
#include "../Headers/Animation.h"

namespace engine {
	Animation::Animation()
	{
		KeyFrame _currentFrame = KeyFrame();
	}

	Mat4 Animation::GetModelMatrix()
	{
		return MatFactory::createTranslationMat4(_currentFrame.translation) * GLRotationMatrix(_currentFrame.rotation) * MatFactory::createScaleMat4(_currentFrame.scale);
	}

	Vec3 Animation::GetScale()
	{
		return _currentFrame.scale;
	}

	Vec3 Animation::GetTranslation()
	{
		return _currentFrame.translation;
	}

	Qtrn Animation::GetRotation()
	{
		return _currentFrame.rotation;
	}

	void  Animation::SetSpeed(float speed)
	{
		this->_speed = speed;
	}

	void Animation::Start()
	{
		// When it starts for the first time, initialize key frames
		if (this->_keyFrames.empty())
			Initialize();

		this->_playing = true;
	}

	void Animation::Pause() {
		this->_playing = false;
	}

	void Animation::Stop() {
		this->_playing = false;
		this->Initialize();
	}

	bool Animation::isPlaying()
	{
		return this->_playing;
	}

	void Animation::Initialize()
	{
		this->_step = 0;
		this->_keyFrames = this->_initialKeyFrames;
		std::reverse(this->_keyFrames.begin(), this->_keyFrames.end());
		this->_currentFrame = this->_keyFrames.back();
	}

	/// <summary>
	/// Adds an neutral KeyFrame to the animation
	/// </summary>
	void Animation::AddKeyFrame()
	{
		KeyFrame keyFrame;
		this->_initialKeyFrames.push_back(keyFrame);
	}

	/// <summary>
	/// Adds a key frame to the animation.
	/// The first key frame is the starting point of the animation.
	/// </summary>
	/// <param name="translation"></param>
	/// <param name="rotation"></param>
	/// <param name="scale"></param>
	void Animation::AddKeyFrame(Vec3 scale, Qtrn rotation, Vec3 translation)
	{
		KeyFrame keyFrame = { scale, rotation, translation };
		this->_initialKeyFrames.push_back(keyFrame);
	}

	void Animation::AddScaleKeyFrame(Vec3 scale)
	{
		KeyFrame lastFrame = this->_initialKeyFrames.back();
		this->AddKeyFrame(scale, lastFrame.rotation, lastFrame.translation);
	}

	void Animation::AddRotationKeyFrame(Qtrn rotation)
	{
		KeyFrame lastFrame = this->_initialKeyFrames.back();
		this->AddKeyFrame(lastFrame.scale, rotation, lastFrame.translation);
	}

	void Animation::AddTranslationKeyFrame(Vec3 translation)
	{
		KeyFrame lastFrame = this->_initialKeyFrames.back();
		this->AddKeyFrame(lastFrame.scale, lastFrame.rotation, translation);
	}

	bool Animation::EqualFrame(KeyFrame frame1, KeyFrame frame2)
	{
		if (frame1.translation != frame2.translation || frame1.rotation != frame2.rotation || frame1.scale != frame2.scale)
			return false;
		else
			return true;
	}

	void Animation::Update(float elapsed_time)
	{
		if (_playing)
		{
			// If all the keyframes have been reached the animation has finished.
			if (_keyFrames.size() == 1) {
				Stop();
				if (looping)
					Start();
				else
					return;
			}

			_step += _speed * elapsed_time;
			_step = _step > 1.0f ? 1.0f : _step;

			KeyFrame startFrame = _keyFrames.back();
			KeyFrame targetFrame = _keyFrames.end()[-2];
			_currentFrame.scale = lerp(startFrame.scale, targetFrame.scale, _step);
			_currentFrame.rotation = slerp(startFrame.rotation, targetFrame.rotation, _step);
			_currentFrame.translation = lerp(startFrame.translation, targetFrame.translation, _step);

			// When a keyframe is reached - pop the stack
			if (EqualFrame(_currentFrame, targetFrame)) {
				_keyFrames.pop_back();
				_step = 0;
			}
		}
	}

}