#pragma once

// GLEW
#include <gl/glew.h>

// OpenGL Header File
#include <GL/gl.h>

// GLEW
#pragma comment(lib, "glew32.lib")

// OpenGL Libraries
#pragma comment(lib, "OpenGL32.lib")

#pragma comment(lib, "assimp-vc143-mtd.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

// 

enum
{
	MATRIX_ATTRIBUTE_POSITION = 0,
	MATRIX_ATTRIBUTE_COLOR,
	MATRIX_ATTRIBUTE_NORMAL,
	MATRIX_ATTRIBUTE_TEXTURE0,
	MATRIX_ATTRIBUTE_TANGENT,
	MATRIX_ATTRIBUTE_BITTANGENT,
	MATRIX_ATTRIBUTE_SEED,
	MATRIX_ATTRIBUTE_VELOCITY
};

enum ClothSimulation
{
	MATRIX_CLOTH_ATTRIBUTE_POSITION_MASS = 0,
	MATRIX_CLOTH_ATTRIBUTE_PREVIOUS_POSITION,
};

enum GLShaderType
{
	VERTEX = 0,
	TESSELLATION_CONTROL,
	TESSELLATION_EVALUATION,
	GEOMETRY,
	FRAGMENT,
	COMPUTE
};
