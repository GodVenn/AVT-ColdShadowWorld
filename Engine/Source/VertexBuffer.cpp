#include "../Headers/pch.h"
#include "../Headers/VertexBuffer.h"

namespace engine 
{
	VertexBuffer::VertexBuffer(const void* data, GLuint size)
	{
		glGenBuffers(1, &rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &rendererID);
	}

	const void VertexBuffer::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	}

	const void VertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}