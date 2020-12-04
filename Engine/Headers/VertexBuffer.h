#pragma once

namespace engine
{
	struct VertexBuffer
	{
		VertexBuffer(const void* data, GLuint size);
		~VertexBuffer();

		const void bind();
		const void unbind();

	private:
		GLuint rendererID;
	};
}