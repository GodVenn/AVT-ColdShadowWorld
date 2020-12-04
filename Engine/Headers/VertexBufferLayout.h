#pragma once

#include "Vector.h"

namespace engine
{
	struct VertexLayoutElement
	{
		GLuint type;
		GLuint count;	
		bool normalized;

		static GLuint GetSizeOfType(GLuint type) 
		{
			switch (type)
			{
				case GL_FLOAT:
					return 4;
				case GL_UNSIGNED_INT:
					return 4;
				case GL_UNSIGNED_BYTE:
					return 1;
				default:
					std::cerr << "Type not recognized" << std::endl;
					return 0;
			}
		}
	};

	struct VertexBufferLayout
	{
		VertexBufferLayout()
			: size(0), element(){}

		template<typename T>
		void setBufferLayout(int count) 
		{
			std::cerr << "Error: VertexBufferLayout don't recognized this type." << std::endl;
		}

		template<>
		void setBufferLayout<float>(int count)
		{
			element = { GL_FLOAT, (GLuint) count, GL_FALSE};
			size += count * VertexLayoutElement::GetSizeOfType(GL_FLOAT);
		}

		template<>
		void setBufferLayout<unsigned int>(int count)
		{
			element = { GL_UNSIGNED_INT, (GLuint)count, GL_FALSE} ;
			size += count * VertexLayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
		}

		template<>
		void setBufferLayout<unsigned char>(int count)
		{
			element = { GL_UNSIGNED_BYTE,(GLuint)count, GL_TRUE};
			size += count * VertexLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);
		}

		const inline VertexLayoutElement getElements() { return element; }
		const inline GLint getSize() { return size; }

	private:
		VertexLayoutElement element;
		GLint size;
	};
}