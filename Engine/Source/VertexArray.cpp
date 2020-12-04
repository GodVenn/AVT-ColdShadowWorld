#include "../Headers/pch.h"
#include "../Headers/VertexArray.h"

namespace engine 
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &rendererID);
	}
	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &rendererID);
	}

	const void VertexArray::bind()
	{
		glBindVertexArray(rendererID);
	}

	const void VertexArray::unbind()
	{
		glBindVertexArray(0);
	}

	void VertexArray::addBuffer(int vertexAttrib, VertexBuffer& vb, VertexBufferLayout& layout)
	{
		bind();
		vb.bind();
		VertexLayoutElement element = layout.getElements();
			
		glEnableVertexAttribArray(vertexAttrib);
		glVertexAttribPointer(vertexAttrib, element.count, element.type, element.normalized, layout.getSize(), 0);
		
	}
}