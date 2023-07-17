#pragma once

#include "./OpenGL/GLHeadersAndMacros.h"
#include "./OpenGL/GLLog.h"

#include "../includes/vmath.h"

enum SCENE
{
    SCENE_OPENING
};

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

// SCENE START TIMINGS

// Version 4 Audio Timings
#define START_TIME_OPENING_1 0.0f // 15 Seconds
#define START_TIME_INTRO_2 15.0f
#define START_TIME_GATE 60.0f
#define START_TIME_STUDIO_3 65.0f         // 73 Seconds
#define START_TIME_AAG_4 138.0f           // 55 Seconds
#define START_TIME_BARSAT_5 193.0f        // 56 Seconds
#define START_TIME_AWARA_6 249.0f         // 48 Seconds
#define START_TIME_SHREE_420_7 297.0f     // 74 Seconds
#define START_TIME_JIS_DESH_MEIN_8 371.0f // 81 Seconds
#define START_TIME_SANGAM_9 452.0f        //

#define START_TIME_OUTRO_10 460.0f

#define START_TIME_MERA_NAAM_JOKER 617.0f

#define START_TIME_OUTRO_AISH (0.0f)
#define START_TIME_OUTRO_AMOGH (4.0f)
#define START_TIME_OUTRO_ATHARV (8.0f)
#define START_TIME_OUTRO_KISHOR (12.0f)
#define START_TIME_OUTRO_MRUNAL (16.0f)
#define START_TIME_OUTRO_PRATIK (20.0f)
#define START_TIME_OUTRO_MADAM (25.0f)
#define START_TIME_OUTRO_RUTVIK (30.0f)
#define START_TIME_OUTRO_SAGAR (35.0f)
#define START_TIME_OUTRO_SAHIL (40.0f)
#define START_TIME_OUTRO_VARUN (45.0f)
#define START_TIME_OUTRO_PRADNYA (50.0f)
#define START_TIME_OUTRO_GUIDANCE (54.5f) // Slides
#define START_TIME_OUTRO_TAUFIK (58.0f)
#define START_TIME_OUTRO_RTRGLS (61.5f)
#define START_TIME_OUTRO_UTKARSH (64.0f)
#define START_TIME_OUTRO_END (68.0f)

#define START_TIME_LOGO_11 480.0f

#define STOP_TIME_RADIOKATA 126.0f

// *** By Pradnya ***
// To create cloth shader program objects differently for each scene - each with "Mass Spring" shader customized specifically for that scene.
// See ClothShader.h => initializeClothMassSpringShader();
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
extern BOOL start_fade_out_opening;

void initializePlaneVAO(const int res, const int width, GLuint *planeVAO, GLuint *planeVBO, GLuint *planeEBO);
vmath::vec3 genRandomVec3();