#include "terrain.hpp"
#include <iostream>
#include <fftw3.h>
#include <time.h>
#include <math.h>


glm::vec3 vecFromArrayAndIndex(GLfloat* array, int index)
{
	return glm::vec3(array[3*index], array[3*index + 1], array[3*index + 2]);
}

glm::vec3 Terrain::normalAt(int x, int z)
{
	// No check for out of bounds to avoid too much overhead cost
	return vecFromArrayAndIndex(_model->normalArray(), x + z*_width);
}

glm::vec3 Terrain::vertexAt(int x, int z)
{
	// No check for out of bounds to avoid too much overhead cost
	return vecFromArrayAndIndex(_model->vertexArray(), x + z*_width);
}


char TerrainGenerator::outOfBounds(int x, int z, int width, int height)
{
	return !(x >= 0 && x < width && z >= 0 && z < height);
}

int TerrainGenerator::textureIndex(int x, int z, int offsetX, int offsetZ)
{
	if (outOfBounds(x + offsetX, z + offsetZ, _textureData.width, _textureData.height))
		return x + z*_textureData.width;
	else
		return x + offsetX + (z + offsetZ)*_textureData.width;
}

Terrain* TerrainGenerator::generateTerrain(unsigned int width, unsigned int depth)
{
	_heightMap = generateHeightMapData(width, depth);
	return generateTerrain();
}

GLfloat* TerrainGenerator::generateHeightMapData(unsigned int width, unsigned int depth)
{
	fftw_complex *in, *out;
	fftw_plan p;

	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * width*depth);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * width*depth);

	srand(time(NULL));
	for (int x = 0; x < width; ++x)
	{
		for (int z = 0; z < depth; ++z)
		{
			int index = x + z*depth;
			// float distance = 1 + sqrt(x*x + z*z);
			double dice_roll = rand() % 1000; 
			dice_roll = dice_roll/500.0 - 1;
			in[index][0] = dice_roll;
			in[index][1] = 0.0;
		}
	}

	p = fftw_plan_dft_2d(width, depth, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

	fftw_execute(p);

	GLfloat* heightMap = (GLfloat*)malloc(sizeof(GLfloat)*width*depth);
	for (int x = 0; x < width; ++x)
	{
		for (int z = 0; z < depth; ++z)
		{
			int index = x + z*depth;
			heightMap[index] = in[index][0];
			// std::cout << " " << (double)in[index][0];
		}
		// std::cout << std::endl;
	}

	fftw_destroy_plan(p);
	fftw_free(in); 
	fftw_free(out);

	return heightMap;
}

Terrain* TerrainGenerator::generateTerrain(const char* filePath)
{
	loadTextureData(filePath);
	return generateTerrain();
}

Terrain* TerrainGenerator::generateTerrain()
{
	initArrays();
	calculatVertices();
	calculateIndices();
	calculateNormalVectors();
	generateModel();
	Terrain* terrain = new Terrain(_model, _textureData);
	releaseMemory();
	return terrain;
}

void TerrainGenerator::loadTextureData(const char* filePath)
{
	LoadTGATextureData((char*)filePath, &_textureData);
	unpackTextureData(&_textureData);
}

void TerrainGenerator::unpackTextureData(TextureData* textureData)
{
	_vertexCount = textureData->width*textureData->height;
	_triangleCount = (textureData->width - 1)*(textureData->height - 1)*2;	
	_heightMap = (GLfloat*)malloc(_vertexCount*sizeof(GLfloat));
	for (int x = 0; x < textureData->width; ++x)
	{
		for (int z = 0; z < textureData->height; ++z)
		{
			unsigned int index = x + z*textureData->width;
			_heightMap[index] = textureData->imageData[index*(textureData->bpp/8)]/256.0 - 1; // Normalizing around [-1, +1]
		}
	}
}

void TerrainGenerator::initArrays()
{
	_vertexArray = (GLfloat*)malloc(3*_vertexCount*sizeof(GLfloat));
	_texCoordArray = (GLfloat*)malloc(2*_vertexCount*sizeof(GLfloat));
	_normalArray = (GLfloat*)malloc(3*_vertexCount*sizeof(GLfloat));
	_indexArray = (GLuint*)malloc(3*_triangleCount*sizeof(GLuint));
}

void TerrainGenerator::calculatVertices()
{
	for (int x = 0; x < _textureData.width; ++x)
	{
		for (int z = 0; z < _textureData.height; ++z)
		{
			unsigned int index = x + z * _textureData.width;

			_vertexArray[index*3 + 0] = x/1.0;
			_vertexArray[index*3 + 1] = 20*_heightMap[index];
			_vertexArray[index*3 + 2] = z/1.0;

			_texCoordArray[index*2 + 0] = x;
			_texCoordArray[index*2 + 1] = z;
		}
	}
}

void TerrainGenerator::calculateIndices()
{
	for (int x = 0; x < _textureData.width - 1; ++x)
	{
		for (int z = 0; z < _textureData.height - 1; ++z)
		{
			// Triangle 1
			_indexArray[(x + z*(_textureData.width-1))*6 + 0] = x + z*_textureData.width;
			_indexArray[(x + z*(_textureData.width-1))*6 + 1] = x + (z + 1)*_textureData.width;
			_indexArray[(x + z*(_textureData.width-1))*6 + 2] = x + 1 + z*_textureData.width;
			// Triangle 2
			_indexArray[(x + z*(_textureData.width-1))*6 + 3] = x + 1 + z*_textureData.width;
			_indexArray[(x + z*(_textureData.width-1))*6 + 4] = x + (z + 1)*_textureData.width;
			_indexArray[(x + z*(_textureData.width-1))*6 + 5] = x + 1 + (z + 1)*_textureData.width;
		}
	}
}

void TerrainGenerator::calculateNormalVectors()
{
	for (int x = 0; x < _textureData.width; ++x)
	{
		for (int z = 0; z < _textureData.height; ++z)
		{

			int index[] = 
			{
				textureIndex(x, z, 0, 0),
				textureIndex(x, z, 1, 0),
				textureIndex(x, z, 1, -1),
				textureIndex(x, z, 0, -1),
				textureIndex(x, z, -1, -1),
				textureIndex(x, z, -1, 0),
				textureIndex(x, z, -1, 1),
				textureIndex(x, z, 0, 1),
				textureIndex(x, z, 1, 1)
			};

			glm::vec3 origin = vecFromArrayAndIndex(_vertexArray, index[0]);
			glm::vec3 normalVector = {0.0f, 0.0f, 0.0f};

			for (int i = 1; i < 8; ++i)
			{

				glm::vec3 firstVector = vecFromArrayAndIndex(_vertexArray, index[i]) - origin;
				glm::vec3 secondVector = vecFromArrayAndIndex(_vertexArray, index[i + 1]) - origin;
				normalVector += glm::cross(firstVector, secondVector);
			}

			normalVector = glm::normalize(normalVector);
			_normalArray[index[0]*3 + 0] = normalVector.x;
			_normalArray[index[0]*3 + 1] = normalVector.y;
			_normalArray[index[0]*3 + 2] = normalVector.z;
		}
	}
}

void TerrainGenerator::generateModel()
{
	Model* tModel = LoadDataToModel(
		_vertexArray,
		_normalArray,
		_texCoordArray,
		NULL,
		_indexArray,
		_vertexCount,
		3*_triangleCount);

	VertexModel* vertexModel = new VertexModel(tModel);
	_model = vertexModel;
}

void TerrainGenerator::releaseMemory()
{
	_vertexCount = 0;
	_triangleCount = 0;
	free(_vertexArray);
	free(_normalArray);
	free(_texCoordArray);
	free(_indexArray);
	free(_heightMap);
	free(_textureData.imageData);
}

Terrain::Terrain(VertexModel* model, TextureData textureData)
{
	_model = model;
	_width = textureData.width;
	_depth = textureData.height;
}


void Terrain::draw(GLuint shaderProgram)
{
	loadMTWMatrixToGPU(shaderProgram);
	_model->drawModel(shaderProgram, "in_Position", "in_Normal", "in_TextureCoordinates");
}

float Terrain::heightAt(float x, float z)
{
	int intPartX = (int)x;
	int intPartZ = (int)z;
	float decimalPartX = x - (float)intPartX;
	float decimalPartZ = z - (float)intPartZ;

	int index = intPartX + intPartZ*_width;

	glm::vec3 vertexLocation(_model->vertexArray()[index*3], _model->vertexArray()[index*3 + 1], _model->vertexArray()[index*3 + 2]);
	glm::vec3 normalVector(_model->normalArray()[index*3], _model->normalArray()[index*3 + 1], _model->normalArray()[index*3 + 2]);
	glm::vec3 movementVector(decimalPartX, 0, decimalPartZ);

	glm::vec3 planeVector = glm::cross(normalVector, movementVector);
	glm::vec3 planeVector2 = glm::cross(planeVector, normalVector);

	return vertexLocation.y + planeVector2.y;
}

Terrain::~Terrain()
{
	delete _model;
}