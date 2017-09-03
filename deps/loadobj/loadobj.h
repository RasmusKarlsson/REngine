#ifndef loadobj_h
#define loadobj_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <Windows.h>
	#include <gl\glew.h>
	#include <GL/freeglut.h>
#endif

typedef struct
{
  GLfloat* vertexArray;
  GLfloat* normalArray;
  GLfloat* texCoordArray;
  GLfloat* colorArray; // Rarely used
  GLuint* indexArray;

  GLfloat* tangentArray;
  GLfloat* bitangentArray;

  int numVertices;
  int numIndices;
  
  // Space for saving VBO and VAO IDs
  GLuint vao; // VAO
  GLuint vb, ib, nb, tb, tab, btb; // VBOs
} Model;

Model* LoadModel(char* name);

// NEW:

void DrawModel(Model *m);
void DrawWireframeModel(Model *m);

Model* LoadModelPlus(char* name,
			GLuint program,
			char* vertexVariableName,
			char* normalVariableName,
			char* texCoordVariableName,

			char* tangentVariableName = NULL,
			char* bitangentVariableName = NULL);

void EnableModelForShader(Model *m, GLuint program, // NOT TESTED
			char* vertexVariableName,
			char* normalVariableName,
			char* texCoordVariableName);
Model* LoadDataToModel(
			GLfloat *vertices,
			GLfloat *normals,
			GLfloat *texCoords,
			GLfloat *colors,
			GLuint *indices,

			GLfloat *tangents,
			GLfloat *bitangents,

			int numVert,
			int numInd,
			
			GLuint program,
			char* vertexVariableName,
			char* normalVariableName,
			char* texCoordVariableName,

			char* tangentVariableName,
			char* bitangentVariableName);

void BuildModelVAO(Model *m,
			GLuint program,
			char* vertexVariableName,
			char* normalVariableName,
			char* texCoordVariableName, 

			char* tangentVariableName,
			char* bitangentVariableName);

void CenterModel(Model *m);

#ifdef __cplusplus
}
#endif

#endif
