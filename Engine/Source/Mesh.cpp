#include "../Headers/pch.h"
#include "../Headers/Mesh.h"

#include <fstream>

namespace engine
{
	Mesh::Mesh()
	{
		this->NormalsLoaded = this->TexcoordsLoaded = false;
		this->VaoId = 0;
	}
	Mesh::Mesh(const std::string& filepath)
	{
		loadMeshData(filepath);
		processMeshData();
		freeMeshData();
		createBufferObjects();
	}
	Mesh::~Mesh()
	{
		destroyBufferObjects();
	}
	void Mesh::parseVertex(std::stringstream& sin)
	{
		Vec3 v;
		sin >> v.x >> v.y >> v.z;
		vertexData.push_back(v);
	}
	void Mesh::parseTexcoord(std::stringstream& sin)
	{
		Vec2 t;
		sin >> t.x >> t.y;
		texcoordData.push_back(t);
	}

	void Mesh::parseNormal(std::stringstream& sin)
	{
		Vec3 n;
		sin >> n.x >> n.y >> n.z;
		normalData.push_back(n);
	}

	void Mesh::parseFace(std::stringstream& sin)
	{
		std::string token;
		if (normalData.empty() && texcoordData.empty())
		{
			for (int i = 0; i < 3; i++)
			{
				sin >> token;
				vertexIdx.push_back(std::stoi(token));
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				std::getline(sin, token, '/');
				if (token.size() > 0) vertexIdx.push_back(std::stoi(token));
				std::getline(sin, token, '/');
				if (token.size() > 0) texcoordIdx.push_back(std::stoi(token));
				std::getline(sin, token, ' ');
				if (token.size() > 0) normalIdx.push_back(std::stoi(token));
			}
		}
	}

	void Mesh::parseLine(std::stringstream& sin)
	{
		std::string s;
		sin >> s;
		if (s.compare("v") == 0) parseVertex(sin);
		else if (s.compare("vt") == 0) parseTexcoord(sin);
		else if (s.compare("vn") == 0) parseNormal(sin);
		else if (s.compare("f") == 0) parseFace(sin);
	}

	void Mesh::loadMeshData(const std::string& filepath)
	{
		std::ifstream ifile(filepath);
		std::string line;
		
		if (ifile.is_open()) 
		{
			while (std::getline(ifile, line))
			{
				std::stringstream sline(line);
				parseLine(sline);
			}
			ifile.close();
			TexcoordsLoaded = (texcoordIdx.size() > 0);
			NormalsLoaded = (normalIdx.size() > 0);
		}
		else 
		{
			std::cerr << "Error: Model file '" << filepath << "'could not be found." << std::endl;
		}

	}

	void Mesh::processMeshData()
	{
		for (unsigned int i = 0; i < vertexIdx.size(); i++) {
			unsigned int vi = vertexIdx[i];
			Vec3 v = vertexData[vi - 1];
			Vertices.push_back(v);
			if (TexcoordsLoaded)
			{
				unsigned int ti = texcoordIdx[i];
				Vec2 t = texcoordData[ti - 1];
				Texcoords.push_back(t);
			}
			if (NormalsLoaded)
			{
				unsigned int ni = normalIdx[i];
				Vec3 n = normalData[ni - 1];
				Normals.push_back(n);
			}
		}
	}

	void Mesh::freeMeshData()
	{
		vertexData.clear();
		texcoordData.clear();
		normalData.clear();
		vertexIdx.clear();
		texcoordIdx.clear();
		normalIdx.clear();
	}

	const void Mesh::draw()
	{
		glBindVertexArray(this->VaoId);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)Vertices.size());
	}

	void Mesh::createBufferObjects()
	{
		GLuint VboVertices, VboTexcoords, VboNormals;
		glGenVertexArrays(1, &VaoId);
		glBindVertexArray(VaoId);
		{
			glGenBuffers(1, &VboVertices);
			glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
			glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vec3), &Vertices[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(Mesh::VERTICES);
			glVertexAttribPointer(Mesh::VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);

			if (TexcoordsLoaded)
			{
				glGenBuffers(1, &VboTexcoords);
				glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
				glBufferData(GL_ARRAY_BUFFER, Texcoords.size() * sizeof(Vec2), &Texcoords[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(Mesh::TEXCOORDS);
				glVertexAttribPointer(Mesh::TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), 0);
			}
			if (NormalsLoaded)
			{
				glGenBuffers(1, &VboNormals);
				glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
				glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Vec3), &Normals[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(Mesh::NORMALS);
				glVertexAttribPointer(Mesh::NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);
			}
		}

		//glBindVertexArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glDeleteBuffers(1, &VboVertices);
		//glDeleteBuffers(1, &VboTexcoords);
		//glDeleteBuffers(1, &VboNormals);
	}

	void Mesh::destroyBufferObjects()
	{
		glBindVertexArray(VaoId);
		glDisableVertexAttribArray(Mesh::VERTICES);
		glDisableVertexAttribArray(Mesh::TEXCOORDS);
		glDisableVertexAttribArray(Mesh::NORMALS);
		glDeleteVertexArrays(1, &VaoId);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}