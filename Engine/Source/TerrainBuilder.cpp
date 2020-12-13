#include "..\Headers\pch.h"
#include "..\Headers\Base.h"
#include "..\Headers\TerrainBuilder.h"
#include <stb_image.h>


namespace engine {

	Vec3 TerrainBuilder::calculateVertex(float indexX, float indexY, float triangleSizeX, float triangleSizeY)
	{
		return Vec3(indexX * triangleSizeX - ((float)terrainSizeX / 2), _heightMap[indexY* simplificationFactor][indexX* simplificationFactor] * maxTerrainHeight, -indexY * triangleSizeY + ((float)terrainSizeY / 2));
	}

	Vec2 TerrainBuilder::calculateTexcoord(float indexX, float indexY, float dimX, float dimY)
	{
		return Vec2((float)indexX / (dimX - 1), (float)indexY / (dimY - 1));
	}

	TerrainBuilder::TerrainBuilder()
	{
		this->terrainMesh = nullptr;
		this->terrainSizeX = 1;
		this->terrainSizeY = 1;
		this->maxTerrainHeight = 1;
		this->simplificationFactor = 1;
	}

	TerrainBuilder::TerrainBuilder(float terrainWidth, float terrainLength, unsigned int simplificationFactor, float maxTerrainHeight)
	{
		this->simplificationFactor = simplificationFactor;
		this->terrainMesh = nullptr;
		this->terrainSizeX = terrainWidth;
		this->terrainSizeY = terrainLength;
		this->maxTerrainHeight = maxTerrainHeight;
	}


	void TerrainBuilder::setHeightMap()
	{
		int dimX = 3;
		int dimY = 3;
		std::vector<float> dim2Vec;
		dim2Vec.push_back(1);
		dim2Vec.push_back(0);
		dim2Vec.push_back(1);
		this->_heightMap.push_back(dim2Vec);
		dim2Vec.clear();
		dim2Vec.push_back(0);
		dim2Vec.push_back(3);
		dim2Vec.push_back(0);
		this->_heightMap.push_back(dim2Vec);
		dim2Vec.clear();
		dim2Vec.push_back(0);
		dim2Vec.push_back(0);
		dim2Vec.push_back(0);
		this->_heightMap.push_back(dim2Vec);
		dim2Vec.clear();

	}


	void TerrainBuilder::setHeightMap(const std::string& Texturefilepath)
	{
#if _DEBUG
		std::cout << "TerrainBuilder: Reading Height Map..." << std::endl;
#endif
		float timeStart = glfwGetTime();
		int height, width, channels;
		//stbi_set_flip_vertically_on_load(1);//Flip the texture make upsidedown
		float* imageBuffer = stbi_loadf(Texturefilepath.c_str(), &width, &height, &channels, 1);

		if (imageBuffer == nullptr)
		{
			std::cout << "ERROR: Could not read 2D texture from path : " << Texturefilepath << " Reason: " << stbi_failure_reason() << std::endl;
			exit(EXIT_FAILURE);
		}

		int size = height * width;
		for (int i = 0; i < height; i++)
		{
			std::vector<float> line;
			for (int j = 0; j < width; j++)
			{
				float columnVal = (float)imageBuffer[i * width + j];
				line.push_back(columnVal);
			}
			this->_heightMap.push_back(line);
		}

		stbi_image_free(imageBuffer);
#if _DEBUG 
		std::cout << "TerrainBuilder: Height Map Read! (" << width << " x " << height << ") (" << glfwGetTime() - timeStart << "s passed)" << std::endl << std::endl;
#endif
	}



	Mesh* TerrainBuilder::buildMesh()
	{
		const int dimY = _heightMap.size() / simplificationFactor;
		const int dimX = _heightMap[0].size() / simplificationFactor;
		const float triangleSizeX = terrainSizeX / (dimX - 1);
		const float triangleSizeY = terrainSizeY / (dimY - 1);

		// Each vertice/point in a LINE a triangle from the other 2 closest points
		// The last line and column are not "origins" of triangles because they are already included in other triangles.
		// Each origin vertex will generate 2 triangles (a quad)
		Vec3 vertex1, vertex2, vertex3, vertex4;
		Vec2 texCoord1, texCoord2, texCoord3, texCoord4;
		std::vector<Vec3> vertices;
		std::vector<Vec2> texcoords;
		std::vector<Vec3> normals;
		float timeStart = glfwGetTime();
#if _DEBUG 
		std::cout << "TerrainBuilder: Building Mesh (" << dimX << " x " << dimY << ")..." << std::endl;
#endif
		for (int i = 0; i < dimY - 1; i++)
		{
			// 1 quad created per iteration here
			for (int j = 0; j < dimX-1; j++)
			{
				// This vertex:
				int indexY = i;
				int indexX = j;
				vertex1 = calculateVertex(indexX, indexY, triangleSizeX, triangleSizeY);
				texCoord1 = calculateTexcoord(indexX, indexY, dimX, dimY);

				// Vertex to the right:
				indexY = i ;
				indexX = j + 1;
				vertex2 = calculateVertex(indexX, indexY, triangleSizeX, triangleSizeY);
				texCoord2 = calculateTexcoord(indexX, indexY, dimX, dimY);

				// Vertex above to the right:
				indexY = i + 1;
				indexX = j + 1;
				vertex3 = calculateVertex(indexX, indexY, triangleSizeX, triangleSizeY);
				texCoord3 = calculateTexcoord(indexX, indexY, dimX, dimY);

				// Vertex above:
				indexY = i + 1;
				indexX = j ;
				vertex4 = calculateVertex(indexX, indexY, triangleSizeX, triangleSizeY);
				texCoord4 = calculateTexcoord(indexX, indexY, dimX, dimY);

				// First triangle
				vertices.push_back(vertex1);
				texcoords.push_back(texCoord1);

				vertices.push_back(vertex2);
				texcoords.push_back(texCoord2);

				vertices.push_back(vertex3);
				texcoords.push_back(texCoord3);

				// Second triangle
				vertices.push_back(vertex1);
				texcoords.push_back(texCoord1);

				vertices.push_back(vertex3);
				texcoords.push_back(texCoord3);
				
				vertices.push_back(vertex4);
				texcoords.push_back(texCoord4);
			}
		}
#if _DEBUG 
		std::cout << "TerrainBuilder: Mesh Built! (" << glfwGetTime() - timeStart << "s passed)" << std::endl << std::endl;
#endif


#if _DEBUG 
		std::cout << "TerrainBuilder: Creating Buffer Object..." << std::endl;
#endif
		timeStart = glfwGetTime();
		this->terrainMesh = new Mesh(vertices, texcoords, normals);
#if _DEBUG 
		std::cout << "TerrainBuilder: Buffer Object Created! (" << glfwGetTime() - timeStart << "s passed)" << std::endl << std::endl;
#endif
		return this->terrainMesh;
	}
}