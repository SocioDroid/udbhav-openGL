#pragma once
#include <stdio.h>
#include <gl\glew.h>
#include <gl\GL.h>

#define PVG_PI 3.14159265358979323846

//function prototype
void GenerateSphere(GLfloat fRadius, GLint iSlices, GLint iStacks, GLfloat **fSphereVertices, GLfloat **fSphereNormals, GLfloat **fSphereTextures, GLushort **uiSphereElements);
void GenerateHemisphere(GLfloat fRadius, GLint iSlices, GLint iStacks, GLfloat **fHemisphereVertices, GLfloat **fHemisphereNormals, GLfloat **fHemisphereTextures, GLushort **uiHemisphereElements);
void GenerateDisk(GLfloat fInnerRadius, GLfloat fOuterRadius, GLint numSlices, GLint numStacks, GLboolean bHalfDisk, GLfloat **fDiskVertices, GLfloat **fDiskNormals, GLfloat **fDiskTextures, GLushort **uiDiskElements);
void GenerateHollowCylinder(GLfloat fBaseRadius, GLfloat fTopRadius, GLfloat fLength, GLint numSlices, GLint numStacks, GLboolean bHalfCylinder, GLfloat **fCylinderVertices, GLfloat **fCylinderNormals, GLfloat **fCylinderTextures, GLushort **uiCylinderElements);
void GenerateSolidCylinder(GLfloat fBaseRadius, GLfloat fTopRadius, GLfloat fLength, GLint numSlices, GLint numStacks, GLboolean bHalfCylinder, GLfloat **fCylinderVertices, GLfloat **fCylinderNormals, GLfloat **fCylinderTextures, GLushort **uiCylinderElements);
void GenerateHollowCone(GLfloat fBaseRadius, GLfloat fLength, GLint numSlices, GLint numStacks, GLboolean bHalfCone, GLfloat **fConeVertices, GLfloat **fConeNormals, GLfloat **fConeTextures, GLushort **uiConeElements);
void GenerateSolidCone(GLfloat fBaseRadius, GLfloat fLength, GLint numSlices, GLint numStacks, GLboolean bHalfCone, GLfloat **fConeVertices, GLfloat **fConeNormals, GLfloat **fConeTextures, GLushort **uiConeElements);
void GenerateTorus(GLfloat fMajorRadius, GLfloat fMinorRadius, GLint numMajor, GLint numMinor, GLboolean bHalfTorus_Lateral, GLboolean bHalfTorus_Longitudinal, GLfloat **fTorusVertices, GLfloat **fTorusNormals, GLfloat **fTorusTextures, GLushort **uiTorusElements);
void GenerateTube(GLfloat fInnerRadius, GLfloat fOuterRadius, GLfloat fLength, GLint numSlices, GLint numStacks, GLfloat fraction_PI_term, GLfloat **fTubeVertices, GLfloat **fTubeNormals, GLfloat **fTubeTextures, GLushort **uiTubeElements);

GLuint GetNumberOfVertices(void);
GLuint GetNumberOfElements(void);
void DeallocateMemoryOfMeshData(GLfloat **fMeshVertices, GLfloat **fMeshNormals, GLfloat **fMeshTextures, GLushort **uiMeshElements);







