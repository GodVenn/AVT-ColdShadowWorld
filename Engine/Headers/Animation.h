#pragma once
#include "Quaternion.h"
#include "Vector.h"
#include "SceneNode.h"

namespace engine
{
	struct KeyFrame {
		Vec3 translation;
		Qtrn rotation;
		Vec3 scale;
		KeyFrame() {
			scale = Vec3(1);
			rotation = Qtrn();
			translation = Vec3();
		}
		KeyFrame(Vec3 inScale, Qtrn inRotation, Vec3 inTranslation) {
			scale = inScale;
			rotation = inRotation;
			translation = inTranslation;
		}
	};

	//struct Rotation {
	//	Rotation();
	//	Rotation(Vec3 axis, float angle, float _timer);
	//	Vec3 axis;
	//	float angle;
	//	float timer;
	//};

	//struct Translation {
	//	Translation();
	//	Vec3 translation;
	//	Vec3 translationCurrent;
	//	float current;
	//};

	class Animation
	{
	private:
		KeyFrame _currentFrame;
		std::vector<KeyFrame> _initialKeyFrames;
		std::vector<KeyFrame> _keyFrames;

		bool _playing = false;
		float _speed = 1.0f;
		float _step = 0.0f;

		bool EqualFrame(KeyFrame frame1, KeyFrame frame2);
		void Initialize();
	public:
		bool looping = false;
		Animation();

		Mat4 GetModelMatrix();
		Vec3 GetScale();
		Vec3 GetTranslation();
		Qtrn GetRotation();

		void SetSpeed(float speed);
		void Start();
		void Pause();
		void Stop();
		bool isPlaying();

		void AddKeyFrame();
		void AddKeyFrame(Vec3 scale, Qtrn rotation, Vec3 translation);
		void AddScaleKeyFrame(Vec3 scale);
		void AddRotationKeyFrame(Qtrn rotation);
		void AddTranslationKeyFrame(Vec3 translation);

		void Update(float elapsed_time);
	};
}