#include "terrain.hpp"

glm::vec3 TerrainGenerator::vecFromArrayAndIndex(GLfloat* array, int index)
{
	return glm::vec3(array[index*3], array[index*3 + 1], array[index*3 + 2]);
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

Terrain* TerrainGenerator::generateTerrain(const char* filePath)
{
	loadTextureData(filePath);
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
	_vertexCount = _textureData.width*_textureData.height;
	_triangleCount = (_textureData.width - 1)*(_textureData.height - 1)*2;
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
			_vertexArray[index*3 + 1] = _textureData.imageData[index*(_textureData.bpp/8)]/20.0;
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
			_indexArray[(x + z * (_textureData.width-1))*6 + 0] = x + z*_textureData.width;
			_indexArray[(x + z * (_textureData.width-1))*6 + 1] = x + (z + 1)*_textureData.width;
			_indexArray[(x + z * (_textureData.width-1))*6 + 2] = x + 1 + z*_textureData.width;
			// Triangle 2
			_indexArray[(x + z * (_textureData.width-1))*6 + 3] = x + 1 + z*_textureData.width;
			_indexArray[(x + z * (_textureData.width-1))*6 + 4] = x + (z + 1)*_textureData.width;
			_indexArray[(x + z * (_textureData.width-1))*6 + 5] = x + 1 + (z + 1)*_textureData.width;
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
				normalVector = glm::cross(firstVector, secondVector);
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

	VertexModel* vertexModel = new VertexModel;
	vertexModel->loadModel(tModel);
	_model = vertexModel;
}

void TerrainGenerator::releaseMemory()
{
	_vertexCount = 0;
	_triangleCount = 0;
	_vertexArray = nullptr;
	_normalArray = nullptr;
	_texCoordArray = nullptr;
	_indexArray = nullptr;
	_model = nullptr;
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

	int index = intPartX + intPartZ*_textureData.width;

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