#pragma once

#include <sstream>

#include "Vector.h"
#include "VertexArray.h"

namespace engine
{
	class Mesh
	{

	public:

		static const GLuint VERTICES = 0;
		static const GLuint TEXCOORDS = 1;
		static const GLuint NORMALS = 2;

		Mesh();
		Mesh(const std::string& filepath);
		~Mesh();

		const void draw();

	private:
		bool TexcoordsLoaded, NormalsLoaded;
		GLuint VaoId;

		std::vector <Vec3> Vertices, vertexData;
		std::vector <Vec2> Texcoords, texcoordData;
		std::vector <Vec3> Normals, normalData;

		std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;

		void parseVertex(std::stringstream& sin);
		void parseTexcoord(std::stringstream& sin);
		void parseNormal(std::stringstream& sin);
		void parseFace(std::stringstream& sin);
		void parseLine(std::stringstream& sin);
		void loadMeshData(const std::string& filepath);
		void processMeshData();
		void freeMeshData();
		void createBufferObjects();
		void destroyBufferObjects();
	};
}

