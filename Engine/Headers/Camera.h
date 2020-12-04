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

		void createUBO(const GLuint UBO);
		void draw();
		void destroyUBO();
		void setViewMatrix(const Vec3& eye, const Vec3& center, const Vec3& up);
		void setOrthograpicProjectionMatrix(const float left, const float right, const float bottom, const float top, const float near, const float far);
		void setPerspectiveProjectionMatrix(const float fovy, const float ratio, const float near, const float far);

		inline const Mat4 getViewMatrix() { return viewMatrix; }
		inline const Mat4 getProjMatrix() { return projMatrix; }

	private:
		Mat4 viewMatrix;
		Mat4 projMatrix;
		GLuint bindingPoint, VboId;
		bool isUBO;
	};
}