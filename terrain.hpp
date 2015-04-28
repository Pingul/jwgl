#ifndef _TERRAIN_
#define _TERRAIN_

#include "models.hpp"
#include "LoadTGA.hpp"
#include <glm/glm.hpp>

class Terrain : public WorldObject
{
	public:
		Terrain(VertexModel* model, TextureData textureData);
		~Terrain();

		virtual bool isAffectedByGravity() { return false; }
		virtual void draw(GLuint shaderProgram);

		float heightAt(float x, float y);
		int width() { return _width; }
		int depth() { return _depth; }

		glm::vec3 normalAt(int x, int z);
		glm::vec3 vertexAt(int x, int z);

	private:
		int _width;
		int _depth;
};

class TerrainGenerator
{
	public:
		TerrainGenerator() = default;
		~TerrainGenerator() = default;

		Terrain* generateTerrain(const char* filePath);
		Terrain* generateTerrain(unsigned int width, unsigned int depth);

	private:
		int _vertexCount;
		int _triangleCount;
		GLfloat* _vertexArray;
		GLfloat* _normalArray;
		GLfloat* _texCoordArray;
		GLuint* _indexArray;
		TextureData _textureData;
		VertexModel* _model;

		char outOfBounds(int x, int z, int width, int height);
		int textureIndex(int x, int z, int offsetX, int offsetZ);
		void loadTextureData(const char* filePath);
		void calculatVertices();
		void calculateIndices();
		void calculateNormalVectors();
		void generateModel();
		void releaseMemory();
		void createTextureData(GLubyte* heightMap, unsigned int width, unsigned int depth);
		void initArrays();

		GLubyte* generateHeightMapData(unsigned int width, unsigned int depth);
		Terrain* generateTerrain();

};

#endif