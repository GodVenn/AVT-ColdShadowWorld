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
		Vec3 calculateVertex(float indexX, float indexY, float triangleSizeX, float triangleSizeY);
		Vec2 calculateTexcoord(float indexX, float indexY, float dimX, float dimY);
	public:
		float terrainSizeX;
		float terrainSizeY;
		float maxTerrainHeight;
		unsigned int simplificationFactor;
		Mesh* terrainMesh;
	
		TerrainBuilder();
		/// <summary>
		/// Provides parameters which will scale the terrain to the desired width, length and height.
		/// </summary>
		/// <param name="terrainWidth">The width (X-axis) of the terrain in units</param>
		/// <param name="terrainLength">The length (Z-axis) of the terrain in units</param>
		/// <param name="simplificationFactor"> The amount of pixels in the height map per vertex in the mesh </param>
		/// <param name="maxTerrainHeight">The max height (Y-axis) of the terrain in units</param>
		TerrainBuilder(float terrainWidth, float terrainLength, unsigned int simplificationFactor, float maxTerrainHeight);
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
		/// Builds the terrain mesh from the height map.
		/// </summary>
		Mesh* buildMesh();
	};

}
