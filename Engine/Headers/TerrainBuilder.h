#pragma once
#include "Mesh.h"
#include "Texture.h"
#include <vector>
#include <random>
#include <time.h>

namespace engine {

	class TerrainBuilder
	{
	private:
		// Height map is a vector of vectors with the structure [line][column] from lower left of image to upper right
		std::vector<std::vector<float>> _heightMap;
		Vec3 calculateVertex(int indexX, int indexY, float triangleSizeX, float triangleSizeY);
		Vec2 calculateTexcoord(int indexX, int indexY, int dimX, int dimY);
		Vec3 calculateTriangleNormal(Vec3 vertex1, Vec3 vertex2, Vec3 vertex3);
		Vec3 calculateVertexNormal(int i, int j, int dimX, int dimY, std::vector<Vec3> triangleNormals);
	public:
		float terrainSizeX;
		float terrainSizeY;
		float maxTerrainHeight;
		unsigned int simplificationFactor;
		bool calculateNormals;
		bool flatShading = true;
		Mesh* terrainMesh;
	
		TerrainBuilder();
		/// <summary>
		/// Provides parameters which will scale the terrain to the desired width, length and height.
		/// </summary>
		/// <param name="terrainWidth">The width (X-axis) of the terrain in units</param>
		/// <param name="terrainLength">The length (Z-axis) of the terrain in units</param>
		/// <param name="simplificationFactor"> The amount of pixels in the height map per vertex in the mesh </param>
		/// <param name="maxTerrainHeight">The max height (Y-axis) of the terrain in units</param>
		TerrainBuilder(float terrainWidth, float terrainLength, unsigned int simplificationFactor, float maxTerrainHeight, bool calculateNormals = true);
		inline std::vector<std::vector<float>> getHeightMap() const { return _heightMap; };

		/// <summary>
		/// Currently creates a 3x3 matrix for testing
		/// </summary>
		void setHeightMap();

		/// <summary>
		/// Converts a texture(image) into a local jagged array (_heightMap) of grayscale values
		/// </summary>
		/// <returns></returns>
		void setHeightMap(const std::string& Texturefilepath);

		/// <summary>
		/// Generate a height map from perlin noise
		/// </summary>
		/// <param name="width">Number of "pixels" in X-direction</param>
		/// <param name="length">Number of "pixels" in Y-direction</param>
		/// <param name="octaves">The number of different octaves that make up the final height map</param>
		/// <param name="persistance">The decreasing factor for amplitude by each octave</param>
		/// <param name="lacunarity">The increasing factor for frequency by each octave</param>
		void generateHeightMap(int width, int length, int octaves, float persistance, float lacunarity, unsigned int seed);

		/// <summary>
		/// Builds the terrain mesh from the height map.
		/// </summary>
		Mesh* buildMesh();
	};

}
