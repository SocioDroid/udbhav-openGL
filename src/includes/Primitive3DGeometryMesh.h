#pragma once
#include <stdlib.h>
#include <math.h>
#include <gl\glew.h>
#include <gl\GL.h>

void AllocateMemoryForMeshData(GLuint iNumIndices);
void AddTriangle(GLfloat single_vertex[3][3], GLfloat single_normal[3][3], GLfloat single_texture[3][2]);
void NormalizeVector(GLfloat v[3]);
bool IsFoundIdentical(const GLfloat value1, const GLfloat value2, const float difference);
void GetMeshData(GLfloat **mesh_vertices, GLfloat **mesh_normals, GLfloat **mesh_textures, GLushort **mesh_elements);

GLuint GetNumberOfVerticesOfMesh(void);
GLuint GetNumberOfElementsOfMesh(void);
void DeallocateMemoryOfModelMeshData(GLfloat **mesh_vertices, GLfloat **mesh_normals, GLfloat **mesh_textures, GLushort **mesh_elements);




