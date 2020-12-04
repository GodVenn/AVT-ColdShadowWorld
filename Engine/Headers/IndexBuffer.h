#pragma once

namespace engine
{
	struct IndexBuffer
	{
		IndexBuffer(const GLubyte* data, const  GLuint count);
		~IndexBuffer();

		const void bind();
		const void unbind();

		const inline GLuint getCount() { return count; }

	private:
		GLuint rendererID;
		GLuint count;
	};
}