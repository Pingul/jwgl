#ifndef loadobj_h
#define loadobj_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/gl.h>
#endif

typedef struct
{
  GLfloat* vertexArray;
  GLfloat* normalArray;
  GLfloat* texCoordArray;
  GLfloat* colorArray; // Rarely used
  GLuint* indexArray;
  int numVertices;
  int numIndices;
  
  // Space for saving VBO and VAO IDs
  GLuint vao; // VAO
  GLuint vb, ib, nb, tb; // VBOs
} Model;

Model* LoadModel(const char* name);

// NEW:

void DrawModel(Model *m, GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName);
void DrawWireframeModel(Model *m, GLuint program, const char* vertexVariableName, const char* normalVariableName, const char* texCoordVariableName);

Model* LoadModelPlus(const char* name/*,
			GLuint program,
			const char* vertexVariableName,
			const char* normalVariableName,
			const char* texCoordVariableName*/);
void EnableModelForShader(Model *m, GLuint program, // NOT TESTED
			const char* vertexVariableName,
			const char* normalVariableName,
			const char* texCoordVariableName);
Model* LoadDataToModel(
			GLfloat *vertices,
			GLfloat *normals,
			GLfloat *texCoords,
			GLfloat *colors,
			GLuint *indices,
			int numVert,
			int numInd);

/*
Model* LoadDataToModel(
			GLfloat *vertices,
			GLfloat *normals,
			GLfloat *texCoords,
			GLfloat *colors,
			GLuint *indices,
			int numVert,
			int numInd,
			
			GLuint program,
			const char* vertexVariableName,
			const char* normalVariableName,
			const char* texCoordVariableName);
void BuildModelVAO(Model *m,
			GLuint program,
			const char* vertexVariableName,
			const char* normalVariableName,
			const char* texCoordVariableName);
*/

void CenterModel(Model *m);
void ScaleModel(Model *m, float sx, float sy, float sz);

#ifdef __cplusplus
}
#endif

#endif
