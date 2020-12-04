#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace engine
{
	struct VertexArray
	{
		VertexArray();
		~VertexArray();

		void addBuffer(int vertexAttrib, VertexBuffer& vb, VertexBufferLayout& layout);

		const void bind();
		const void unbind();
	
	private:
		GLuint rendererID;
	};
}