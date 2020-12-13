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

		/// <summary>
		/// Creates a mesh with buffer object from an .obj file
		/// </summary>
		/// <param name="filepath"></param>
		Mesh(const std::string& filepath);

		/// <summary>
		/// Creates a mesh with buffer object from the vertex data passed as arguments
		/// </summary>
		/// <param name="Vertices"></param>
		/// <param name="Texcoords"></param>
		/// <param name="Normals"></param>
		Mesh(std::vector <Vec3> Vertices, std::vector <Vec2> Texcoords, std::vector <Vec3> Normals);
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

