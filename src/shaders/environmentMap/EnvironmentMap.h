// #pragma once
// #include "../../utils/OpenGL/GLHeadersAndMacros.h"
// #include "../../utils/OpenGL/GLShaders.h"
// #include "../../utils/common.h"

// class EnvMapShader
// {
// public:
//     // Variables

//     //--- Chrome EnvMap Texture---//
//     GLuint shaderProgramObject_chromeMap;

//     GLuint projectionMatrixUniform_chromeMap;
//     GLuint viewMatrixUniform_chromeMap;
//     GLuint modelMatrixUniform_chromeMap;
//     GLuint textureSamplerUniform_chromeMap;
//     GLuint ktxTexture_chromeMap1;
//     GLuint ktxTexture_chromeMap2;
//     GLuint ktxTexture_chromeMap3_Chrome;

//     //--- SkyBox Map---//
//     GLuint shaderProgramObject_skyBox;
//     GLuint projectionMatrixUniform_skyBox;
//     GLuint viewMatrixUniform_skyBox;
//     GLuint modelMatrixUniform_skyBox;
//     unsigned int cubemapTexture_SkyBox;
//     GLuint vao_cube_sky;
//     GLuint vbo_cube_position_sky;

//     //----CubeMapReflect------//
//     GLuint shaderProgramObject_CubeMapReflect;
//     GLuint modelMatrixUniform_CubeMapReflect;
//     GLuint viewMatrixUniform_CubeMapReflect;
//     GLuint projectionMatrixUniform_CubeMapReflect;
//     GLuint eyePositionUniform_CubeMapReflect;
//     GLuint vao_cube_Reflect;
//     GLuint vbo_cube_position_Reflect;

//     float fEyeX = 0.0f;
//     float fEyeY = 0.0f;
//     float fEyeZ = 3.0f;

//     vec3 eyePosition = vmath::vec3(fEyeX, fEyeY, fEyeZ);
//     vec3 center = vmath::vec3(0.0f, 0.0f, 0.0f);
//     vec3 up = vmath::vec3(0.0f, 1.0f, 0.0f);

//     //---Surface Quad Reflection---
//     #define REFLECTION_FBO_WIDTH 1920
//     #define REFLECTION_FBO_HEIGHT 1080
//     GLuint vao_SurfaceQuad;
//     GLuint vbo_SurfaceQuad_Position;
//     GLuint shaderProgramObject_SurfaceQuad;
//     GLuint modelMatrixUniform_SurfaceQuad;
//     GLuint viewMatrixUniform_SurfaceQuad;
//     GLuint projectionMatrixUniform_SurfaceQuad;

//     GLuint reflectionTextureSampler_SurfaceQuad;
//     vec3 cameraPositions = vec3(0.0f, 1.0f, 2.0f);
//     vec3 centerPositions = vec3(0.0f, 0.0f, 0.0f);
//     vec3 upVectorPositions = vec3(0.0f, 1.0f, 0.0f);

//     //FBO related variables:
//     GLuint fbo_reflection;
//     GLuint rbo_reflection;
//     GLuint fbo_texture_reflection;

//     BOOL initialize(void)
//     {
//         //prototypes:
//         BOOL initialize_chromeMap(void);
//         BOOL initialize_skyBox(void);
//         BOOL initialize_CubeMapReflect(void);
//         BOOL initialize_SurfaceQuad(void);

//         //code:

//         initialize_chromeMap();

//     }

//     BOOL initialize_chromeMap(void)
//     {
//         // vertex Shader
//         GLuint vertexShaderObject = CreateAndCompileShaderObjects("shaders/simpleEnvMap.vs", VERTEX);

//         // fragment Shader
//         GLuint fragmentShaderObject = CreateAndCompileShaderObjects("shaders/simpleEnvMap.fs", FRAGMENT);

//         shaderProgramObject_chromeMap = glCreateProgram();
//         glAttachShader(shaderProgramObject_chromeMap, vertexShaderObject);
//         glAttachShader(shaderProgramObject_chromeMap, fragmentShaderObject);

//         // prelinked binding
//         glBindAttribLocation(shaderProgramObject_chromeMap, MATRIX_ATTRIBUTE_POSITION, "a_position");
//         glBindAttribLocation(shaderProgramObject_chromeMap, MATRIX_ATTRIBUTE_NORMAL, "a_normal");

//         // link
//         BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject_chromeMap);

//         if (bShaderLinkStatus == FALSE)
//             return FALSE;

//         //post link + error checking
//         modelMatrixUniform_chromeMap = glGetUniformLocation(shaderProgramObject_chromeMap, "u_modelMatrix");
//         viewMatrixUniform_chromeMap = glGetUniformLocation(shaderProgramObject_chromeMap, "u_viewMatrix");
//         projectionMatrixUniform_chromeMap = glGetUniformLocation(shaderProgramObject_chromeMap, "u_projectionMatrix");

//         textureSamplerUniform_chromeMap = glGetUniformLocation(shaderProgramObject_chromeMap, "u_envmapSampler");

//         return TRUE;
//     }

//     void displayChromeMapShape(void)
//     {
//         //code:

//     }

//     void uninitialize(void)
//     {
//         UninitializeShaders(shaderProgramObject_chromeMap);
//         UninitializeShaders(shaderProgramObject_skyBox);
//         UninitializeShaders(shaderProgramObject_CubeMapReflect);
//         UninitializeShaders(shaderProgramObject_SurfaceQuad);
//     }
// };
