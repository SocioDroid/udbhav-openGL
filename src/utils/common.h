#pragma once

#include "./OpenGL/GLHeadersAndMacros.h"
#include "./OpenGL/GLLog.h"

#include "../includes/vmath.h"

enum SCENE
{
    SCENE_01_EARTH_BIRTH,
    SCENE_02_EARTH_COOLDOWN,
    SCENE_03_TERRAIN_WITH_HEAVY_RAIN,
    SCENE_04_TERRAIN_SHADOW,
};

// SCENE START TIMINGS

#define START_TIME_SCENE_01_01_INTRO 0.0f
#define START_TIME_SCENE_01_02_EARTH_BIRTH 20.0f
#define START_TIME_SCENE_01_03_EARTH_BIRTH_BLANK 53.0f
#define START_TIME_SCENE_02_01_EARTH_COOLDOWN 64.0f
#define START_TIME_SCENE_03_01_TERRAIN_FIRST_RAIN 115.0f
#define START_TIME_SCENE_04_TERRAIN_SHADOW 157.0f
#define START_TIME_SCENE_04_TERRAIN_SHADOW_START 183.0f
#define START_TIME_SCENE_04_TERRAIN_SHADOW_END 183.0f + 41.0f

enum PHASE
{
    PHASE_MAIN,
};

#include "../shaders/CommonShaders.h"
#include "../shaders/CommonModels.h"
#include "../includes/3DModels.h"

#include "../includes/stb_image.h"

using namespace vmath;

#define MODEL_VIEW_MATRIX_STACK 100

extern mat4 perspectiveProjectionMatrix;

extern vec3 eye;
extern vec3 center;
extern vec3 up;

extern mat4 modelMatrix;
extern mat4 viewMatrix;

extern mat4 matrixStack[MODEL_VIEW_MATRIX_STACK];
extern int matrixStackTop;

// extern Camera camera;
extern std::vector<std::vector<float>> bezierPoints;
extern std::vector<float> yawGlobal;
extern std::vector<float> pitchGlobal;
extern std::vector<float> fovGlobal;
extern BOOL USE_FPV_CAM;
extern int PHASE_CURRENT;
extern float objX;
extern float objY;
extern float objZ;
extern float objIncrement;

extern float scaleX;
extern float scaleY;
extern float scaleZ;
extern float scaleIncrement;

extern float objAngle;
extern float objAngleIncrement;

extern float globalTime;
extern float ELAPSED_TIME;

extern GLuint giWindowWidth;
extern GLuint giWindowHeight;

extern CommonShaders *commonShaders;
extern CommonModels *commonModels;

extern GLuint FBO_DOF;
extern BOOL isDOFTrue;
// extern GLuint fbo_UserMap;

extern float light_objX;
extern float light_objY;
extern float light_objZ;

#define FBO_WIDTH_LIGHT 2048
#define FBO_HEIGHT_LIGHT 2048

BOOL LoadPNGImage(GLuint *texture, const char *imageFilePath);
BOOL LoadGLTexture(GLuint *texture, TCHAR ImageResourceID[]);
BOOL Load1DPNGImage(GLuint *texture, float *data);
unsigned int loadCubemap(const char *faces[]);

void initializeMatrixStack(void);
void pushMatrix(mat4 matrix);
mat4 popMatrix(void);

float degToRad(float degrees);

float osc(float time, float minRange, float maxRange, float frequency);
void setSelectedScene(int selScene);
void setCurrentPhase(int phase);
void updateGlobalViewMatrix();
extern BOOL start_fade_out_opening;

void initializePlaneVAO(const int res, const int width, GLuint *planeVAO, GLuint *planeVBO, GLuint *planeEBO);
vmath::vec3 genRandomVec3();
