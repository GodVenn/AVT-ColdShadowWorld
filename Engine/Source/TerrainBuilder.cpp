#include "..\Headers\pch.h"
#include "..\Headers\Base.h"
#include "..\Headers\TerrainBuilder.h"
#include "..\Headers\PerlinNoise.h"
#include <stb_image.h>
#include <map>
#include <limits>


namespace engine {

	/// <summary>
	/// Different base heights result in different steepness to simulate real terrain.
	/// </summary>
	/// <param name="mapHeight"></param>
	/// <returns></returns>
	float heightCurve(float mapHeight, float maxHeight)
	{
		const float groundLevel = 0.3f;
		float meshHeight;

		if (mapHeight < groundLevel)
			meshHeight = mapHeight;
		else
			meshHeight = mapHeight * mapHeight * maxHeight;

		return meshHeight;
	}

	Vec3 TerrainBuilder::calculateVertex(int indexX, int indexY, float triangleSizeX, float triangleSizeY)
	{
		int i = indexY * simplificationFactor;
		int j = indexX * simplificationFactor;

		ASSERT(i < _heightMap.size() || j < _heightMap[0].size(), "Height map access out of bounds for terrain generation.");

		float inputHeight = _heightMap[i][j];
		return Vec3(indexX * triangleSizeX - ((float)terrainSizeX / 2), heightCurve(inputHeight, maxTerrainHeight), -indexY * triangleSizeY + ((float)terrainSizeY / 2));
	}

	Vec2 TerrainBuilder::calculateTexcoord(int indexX, int indexY, int dimX, int dimY)
	{
		return Vec2((float)indexX / (dimX - 1), (float)indexY / (dimY - 1));
	}

	Vec3 TerrainBuilder::calculateTriangleNormal(Vec3 vertex1, Vec3 vertex2, Vec3 vertex3)
	{
		Vec3 v1 = vertex3 - vertex2;
		Vec3 v2 = vertex1 - vertex2;

		return normalize(v1.crossProduct(v2));
	}

	Vec3 TerrainBuilder::calculateVertexNormal(int i, int j, int dimX, int dimY, std::vector<Vec3> triangleNormals)
	{
		std::vector<Vec3> normals;
		int triangleIndex = 0;

		// Triangle lower left far
		int indexX = 2 * j - 2;
		int indexY = i - 1;
		if (indexX >= 0 && indexY >= 0) {
			triangleIndex = indexX + indexY * (dimX - 1) * 2;
			normals.push_back(triangleNormals[triangleIndex]);
		}

		// Triangle lower left near
		indexX = 2 * j - 1;
		indexY = i - 1;
		if (indexX >= 0 && indexY >= 0) {
			triangleIndex = indexX + indexY * (dimX - 1);
			normals.push_back(triangleNormals[triangleIndex]);
		}

		// Triangle lower right
		indexX = 2 * j;
		indexY = i - 1;
		if (indexY >= 0 && indexX < dimX) {
			triangleIndex = indexX + indexY * (dimX + 1);
			normals.push_back(triangleNormals[triangleIndex]);
		}

		// Triangle upper right far
		indexX = 2 * j + 1;
		indexY = i;
		if (indexX < dimX) {
			triangleIndex = indexX + indexY * (dimX + 1);
			normals.push_back(triangleNormals[triangleIndex]);
		}

		// Triangle upper right near
		indexX = 2 * j;
		indexY = i;
		if (indexX < dimX) {
			triangleIndex = indexX + indexY * (dimX + 1);
			normals.push_back(triangleNormals[triangleIndex]);
		}

		// Triangle upper left
		indexX = 2 * j - 1;
		indexY = i;
		if (indexX >= 0) {
			triangleIndex = indexX + indexY * (dimX + 1);
			normals.push_back(triangleNormals[triangleIndex]);
		}

		// Calculate average
		float weight = 1.0f / (float)normals.size();
		Vec3 avgNormal = Vec3(0);
		for (Vec3 normal : normals)
		{
			avgNormal += weight * normal;
		}
		return normalize(avgNormal);
	}

	TerrainBuilder::TerrainBuilder()
	{
		this->terrainMesh = nullptr;
		this->terrainSizeX = 1;
		this->terrainSizeY = 1;
		this->maxTerrainHeight = 1;
		this->simplificationFactor = 1;
		this->calculateNormals = true;
	}

	TerrainBuilder::TerrainBuilder(float terrainWidth, float terrainLength, unsigned int simplificationFactor, float maxTerrainHeight, bool calculateNormals)
	{
		this->calculateNormals = calculateNormals;
		this->simplificationFactor = std::max(simplificationFactor, (unsigned int)1);
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
		double timeStart = glfwGetTime();
#endif
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

	void TerrainBuilder::generateHeightMap(int width, int length, int octaves, float persistance, float lacunarity, unsigned int seed = 0)
	{
#if _DEBUG
		std::cout << "TerrainBuilder: Generating Height Map..." << std::endl;
		double timeStart = glfwGetTime();
#endif
		
		float minNoiseValue = std::numeric_limits<float>::max();
		float maxNoiseValue = std::numeric_limits<float>::min();

		PerlinNoise perlin;
		if (seed > 0)
			perlin = PerlinNoise(seed);

		for (int y = 0; y < length; y++){
			std::vector<float> line;

			for (int x = 0; x < width; x++)
			{
				float frequency = 1;
				float amplitude = 1;
				float noiseHeight = 0;

				for (int i = 0; i < octaves; i++)
				{
					float noiseSampleX = (x / (float)width) * frequency;
					float noiseSampleY = (y / (float)length) * frequency;
					
					float perlinHeight = (float)perlin.noise(noiseSampleX, noiseSampleY, 0);					
					noiseHeight += perlinHeight * amplitude;

					frequency *= lacunarity;
					amplitude *= persistance;
				}
				if (noiseHeight > maxNoiseValue) {
					maxNoiseValue = noiseHeight;
				}
				else if (noiseHeight < minNoiseValue) {
					minNoiseValue = noiseHeight;
				}
				line.push_back(noiseHeight);
			}
			_heightMap.push_back(line);
		}
		for (int y = 0; y < length; y++) {
			for (int x = 0; x < width; x++)
			{
				// Inverse lerp to normalize height map
				_heightMap[x][y] = (_heightMap[x][y] - minNoiseValue) / (maxNoiseValue - minNoiseValue);
			}
		}

#if _DEBUG 
		std::cout << "TerrainBuilder: Height Map Generated! (" << width << " x " << length << ") (" << glfwGetTime() - timeStart << "s passed)" << std::endl << std::endl;
#endif
	}

	Mesh* TerrainBuilder::buildMesh()
	{
#if _DEBUG 
		int dictionaryCount = 0;
		int calculateCount = 0;
#endif
		const int dimY = static_cast<int>(_heightMap.size()) / simplificationFactor;
		const int dimX = static_cast<int>(_heightMap[0].size()) / simplificationFactor;
		const float triangleSizeX = terrainSizeX / (dimX - 1);
		const float triangleSizeY = terrainSizeY / (dimY - 1);

		// Each vertice/point in a LINE forms a triangle from the other 2 closest points
		// The last line and column are not "origins" of triangles because they are already included in other triangles.
		// Each origin vertex (iteration) will generate 2 triangles (a quad)
		Vec3 vertex1, vertex2, vertex3, vertex4, normal;
		Vec2 texCoord1, texCoord2, texCoord3, texCoord4;
		std::vector<Vec3> vertices;
		std::vector<Vec2> texcoords;
		std::vector<Vec3> normals;
		std::vector<Vec3> triangleNormals;
		std::map<int, Vec3> vertexNormals;
#if _DEBUG 
		std::cout << "TerrainBuilder: Building Mesh (" << dimX << " x " << dimY << ")..." << std::endl;
		double timeStart = glfwGetTime();
#endif

		for (int i = 0; i < dimY - 1; i++)
		{
			// 1 quad created per iteration here
			for (int j = 0; j < dimX - 1; j++)
			{
				// This vertex:
				int indexY1 = i;
				int indexX1 = j;
				vertex1 = calculateVertex(indexX1, indexY1, triangleSizeX, triangleSizeY);
				texCoord1 = calculateTexcoord(indexX1, indexY1, dimX, dimY);

				// Vertex to the right:
				int indexY2 = i;
				int indexX2 = j + 1;
				vertex2 = calculateVertex(indexX2, indexY2, triangleSizeX, triangleSizeY);
				texCoord2 = calculateTexcoord(indexX2, indexY2, dimX, dimY);

				// Vertex above to the right:
				int indexY3 = i + 1;
				int indexX3 = j + 1;
				vertex3 = calculateVertex(indexX3, indexY3, triangleSizeX, triangleSizeY);
				texCoord3 = calculateTexcoord(indexX3, indexY3, dimX, dimY);

				// Vertex above:
				int indexY4 = i + 1;
				int indexX4 = j;
				vertex4 = calculateVertex(indexX4, indexY4, triangleSizeX, triangleSizeY);
				texCoord4 = calculateTexcoord(indexX4, indexY4, dimX, dimY);

				if (this->calculateNormals) {
					Vec3 normal1 = calculateTriangleNormal(vertex1, vertex2, vertex3);
					Vec3 normal2 = calculateTriangleNormal(vertex1, vertex3, vertex4);
					if (flatShading) {
						normals.push_back(normal1);
						normals.push_back(normal1);
						normals.push_back(normal1);
						normals.push_back(normal2);
						normals.push_back(normal2);
						normals.push_back(normal2);
					}
					else {
						// Triangle Normals (Reverse order for later calculation)
						triangleNormals.push_back(normal2);
						triangleNormals.push_back(normal1);
					}

				}

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
		std::cout << "TerrainBuilder: Vertices Created! (" << glfwGetTime() - timeStart << "s passed)" << std::endl << std::endl;
#endif

		if (this->calculateNormals && !this->flatShading)
		{
#if _DEBUG 
			std::cout << "TerrainBuilder: Calculating vertex normals..." << std::endl;
			timeStart = glfwGetTime();
#endif
			// Calculate normals
			for (int i = 0; i < dimY - 1; i++)
			{
#if _DEBUG 
				if (i == dimY / 2)
					std::cout << "TerrainBuilder: 50%... (" << glfwGetTime() - timeStart << "s passed)" << std::endl;
#endif
				for (int j = 0; j < dimX - 1; j++)
				{
					// This vertex:
					int indexY1 = i;
					int indexX1 = j;

					// Vertex to the right:
					int indexY2 = i;
					int indexX2 = j + 1;

					// Vertex above to the right:
					int indexY3 = i + 1;
					int indexX3 = j + 1;

					// Vertex above:
					int indexY4 = i + 1;
					int indexX4 = j;


					// First triangle
					int normX = indexX1;
					int normY = indexY1;
					normal;
					if (vertexNormals.find(normX + i * normY) == vertexNormals.end())
					{
						normal = calculateVertexNormal(normY, normX, dimX, dimY, triangleNormals);
						vertexNormals[normX + i * normY] = normal;
#if _DEBUG
						calculateCount++;
#endif
					}
					else
					{
						normal = vertexNormals[normX + i * normY];
#if _DEBUG
						dictionaryCount++;
#endif

					}
					normals.push_back(normal);

					normX = indexX2;
					normY = indexY2;
					normal;
					if (vertexNormals.find(normX + i * normY) == vertexNormals.end())
					{
						normal = calculateVertexNormal(normY, normX, dimX, dimY, triangleNormals);
						vertexNormals[normX + i * normY] = normal;
#if _DEBUG
						calculateCount++;
#endif

					}
					else
					{
						normal = vertexNormals[normX + i * normY];
#if _DEBUG
						dictionaryCount++;
#endif

					}
					normals.push_back(normal);

					normX = indexX3;
					normY = indexY3;
					if (vertexNormals.find(normX + i * normY) == vertexNormals.end())
					{
						normal = calculateVertexNormal(normY, normX, dimX, dimY, triangleNormals);
						vertexNormals[normX + i * normY] = normal;
#if _DEBUG
						calculateCount++;
#endif

					}
					else
					{
						normal = vertexNormals[normX + i * normY];
#if _DEBUG
						dictionaryCount++;
#endif
					}
					normals.push_back(normal);

					// Second triangle
					normX = indexX1;
					normY = indexY1;
					normal;
					if (vertexNormals.find(normX + i * normY) == vertexNormals.end())
					{
						normal = calculateVertexNormal(normY, normX, dimX, dimY, triangleNormals);
						vertexNormals[normX + i * normY] = normal;
#if _DEBUG
						calculateCount++;
#endif
					}
					else
					{
						normal = vertexNormals[normX + i * normY];
#if _DEBUG
						dictionaryCount++;
#endif
					}
					normals.push_back(normal);


					normX = indexX3;
					normY = indexY3;
					normal;
					if (vertexNormals.find(normX + i * normY) == vertexNormals.end())
					{
						normal = calculateVertexNormal(normY, normX, dimX, dimY, triangleNormals);
						vertexNormals[normX + i * normY] = normal;
#if _DEBUG
						calculateCount++;
#endif
					}
					else
					{
						normal = vertexNormals[normX + i * normY];
#if _DEBUG
						dictionaryCount++;
#endif
					}
					normals.push_back(normal);

					normX = indexX4;
					normY = indexY4;
					normal;
					if (vertexNormals.find(normX + i * normY) == vertexNormals.end())
					{
						normal = calculateVertexNormal(normY, normX, dimX, dimY, triangleNormals);
						vertexNormals[normX + i * normY] = normal;
#if _DEBUG
						calculateCount++;
#endif
					}
					else
					{
						normal = vertexNormals[normX + i * normY];
#if _DEBUG
						dictionaryCount++;
#endif
					}
					normals.push_back(normal);
				}
			}
#if _DEBUG 
			std::cout << "TerrainBuilder: Normals calculated! (" << glfwGetTime() - timeStart << "s passed)" << std::endl;
			float percentCalc = 100 * (float)calculateCount / (calculateCount + dictionaryCount);
			float percentDict = 100 * (float)dictionaryCount / (calculateCount + dictionaryCount);
			std::cout << "TerrainBuilder: " << percentCalc << "% calculated and " << percentDict << "% retrieved from dict" << std::endl << std::endl;
#endif
		}

#if _DEBUG 
		std::cout << "TerrainBuilder: Creating Buffer Object..." << std::endl;
		timeStart = glfwGetTime();
#endif
		this->terrainMesh = new Mesh(vertices, texcoords, normals);
#if _DEBUG 
		std::cout << "TerrainBuilder: Buffer Object Created! (" << glfwGetTime() - timeStart << "s passed)" << std::endl << std::endl;
#endif
		this->_heightMap.clear();
		return this->terrainMesh;
	}
}