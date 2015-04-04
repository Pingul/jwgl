#ifndef _TERRAIN_
#define _TERRAIN_

#include "models.hpp"
#include "LoadTGA.hpp"
#include <glm/glm.hpp>

class Terrain : public WorldObject
{
	public:
		Terrain(VertexModel* model, TextureData textureData) :
			_textureData(textureData) { _model = model; }
		~Terrain();

		virtual bool isAffectedByGravity() { return false; }
		virtual void draw(GLuint shaderProgram);
		float heightAt(float x, float y);

	private:
		TextureData _textureData;
};

class TerrainGenerator
{
	public:
		TerrainGenerator() = default;
		~TerrainGenerator() = default;

		Terrain* generateTerrain(const char* filePath);

	private:
		int _vertexCount;
		int _triangleCount;
		GLfloat* _vertexArray;
		GLfloat* _normalArray;
		GLfloat* _texCoordArray;
		GLuint* _indexArray;
		TextureData _textureData;
		VertexModel* _model;

		glm::vec3 vecFromArrayAndIndex(GLfloat* array, int index);
		char outOfBounds(int x, int z, int width, int height);
		int textureIndex(int x, int z, int offsetX, int offsetZ);
		void loadTextureData(const char* filePath);
		void calculatVertices();
		void calculateIndices();
		void calculateNormalVectors();
		void generateModel();
};

#endif