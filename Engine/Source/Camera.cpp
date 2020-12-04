#include "../Headers/pch.h"
#include "../Headers/Camera.h"

namespace engine
{
	Camera::Camera()
	{
		isUBO = false;
		viewMatrix = projMatrix = Mat4();
		VboId = bindingPoint = 0;
	}

	Camera::Camera(const Vec3& eye, const Vec3& center, const Vec3& up)
	{
		isUBO = false;
		this->viewMatrix = MatFactory::createViewMatrix(eye, center, up);
		VboId = bindingPoint = 0;
	}

	Camera::Camera(const GLuint bindingPoint)
	{
		isUBO = true;
		createUBO(bindingPoint);
	}

	Camera::Camera(const Vec3& eye, const Vec3& center, const Vec3& up, const GLuint bindingPoint)
	{
		isUBO = true;
		this->viewMatrix = MatFactory::createViewMatrix(eye, center, up);
		createUBO(bindingPoint);
	}

	Camera::~Camera()
	{
		if (isUBO) 
		{
			destroyUBO();
		}
		
	}

	void Camera::createUBO(const GLuint bindingPoint)
	{
		this->bindingPoint = bindingPoint;
		/// CREATE UNIFORM BUFFER
		glGenBuffers(1, &VboId);
		glBindBuffer(GL_UNIFORM_BUFFER, VboId);
		{

			glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 16 * 2, 0, GL_STREAM_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, VboId);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

	}

	void Camera::draw()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, this->VboId);
		{
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat) * 16, getViewMatrix().data);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 16, sizeof(GLfloat) * 16, getProjMatrix().data);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void Camera::destroyUBO()
	{
		glDeleteBuffers(1, &VboId);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void Camera::setViewMatrix(const Vec3& eye, const Vec3& center, const Vec3& up)
	{
		this->viewMatrix = MatFactory::createViewMatrix(eye, center, up);
	}

	void Camera::setOrthograpicProjectionMatrix(const float left, const float right, const float bottom, const float top, const float near, const float far)
	{
		this->projMatrix = MatFactory::createOrthographicProjectionMatrix(left, right, bottom, top, near, far);
	}

	void Camera::setPerspectiveProjectionMatrix(const float fovy, const float ratio, const float near, const float far)
	{
		this->projMatrix = MatFactory::createPerspectiveProjectionMatrix(fovy, ratio, near, far);
	}


}