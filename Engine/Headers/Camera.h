#pragma once
#include "Matrix.h"

namespace engine 
{
	class Camera
	{
	public:
		Camera();
		Camera(const Vec3& eye, const Vec3& center, const Vec3& up);
		Camera(const GLuint bindingPoint);
		Camera(const Vec3& eye, const Vec3& center, const Vec3& up, const GLuint bindingPoint);
		~Camera();

		Vec3 eye;
		Vec3 center;
		Vec3 up;

		void createUBO(const GLuint UBO);
		void draw();
		void destroyUBO();
		void setViewMatrix(const Vec3& eye, const Vec3& center, const Vec3& up);
		void setViewMatrix(const Mat4& viewMatrix);
		void setOrthographicProjectionMatrix(const float left, const float right, const float bottom, const float top, const float near, const float far);
		void setPerspectiveProjectionMatrix(const float fovy, const float ratio, const float near, const float far);
		void setProjectionMatrix(const Mat4& projectionMatrix);

		const Mat4 getViewMatrix();
		inline const Mat4 getProjMatrix() { return projMatrix; }
		inline GLuint getBindingPoint() const { return VboId; }

	private:
		Mat4 viewMatrix;
		Mat4 projMatrix;
		GLuint bindingPoint, VboId;
		bool isUBO;
		Vec3 _previousEye;
		Vec3 _previousCenter;
		Vec3 _previousUp;
	};
}