#pragma once
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLShaders.h"

class RainShader
{
public:
    // Variables
    GLuint shaderProgramObject;
    GLuint projectionMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint modelMatrixUniform; // we may need this

    GLuint eyePosUniform;
    GLuint windDirUniform;
    GLuint dtUniform;
    GLuint rainTextureSamplerUniform;
    GLuint rainfactorsSamplerUniform;

    GLuint sunDirUniform;
    GLuint sunColorUniform;
    GLuint sunIntensityUniform;

    // For light
    GLuint lightPositionUniform;
    GLuint laUniform;
    GLuint ldUniform;
    GLuint lsUniform;
    GLuint kaUniform;
    GLuint kdUniform;
    GLuint ksUniform;
    GLuint materialShinessUniform;
    GLuint alphaUniform;

    // Member Functions
    BOOL initializeRainShader()
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\rain\\rain.vs", VERTEX);

        // geometry shader
        GLuint geometryShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\rain\\rain.gs", GEOMETRY);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\rain\\rain.fs", FRAGMENT);

        shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, geometryShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // prelinked binding
        // Binding Position Array
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_POSITION, "positionMC");
        // Binding Seed Array
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_SEED, "vertexSeed");
        // Binding Velocity Array
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_VELOCITY, "vertexVelo");

        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject);

        if (bShaderLinkStatus == FALSE)
            return FALSE;

        // post link - getting
        projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
        viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
        modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");

        eyePosUniform = glGetUniformLocation(shaderProgramObject, "eyePosition");
        windDirUniform = glGetUniformLocation(shaderProgramObject, "windDir");
        dtUniform = glGetUniformLocation(shaderProgramObject, "dt");
        rainTextureSamplerUniform = glGetUniformLocation(shaderProgramObject, "rainTex");
        rainfactorsSamplerUniform = glGetUniformLocation(shaderProgramObject, "rainfactors");
        sunDirUniform = glGetUniformLocation(shaderProgramObject, "sunDir");
        sunColorUniform = glGetUniformLocation(shaderProgramObject, "sunColor");
        sunIntensityUniform = glGetUniformLocation(shaderProgramObject, "sunIntensity");

        // for light
        laUniform = glGetUniformLocation(shaderProgramObject, "u_la");
        ldUniform = glGetUniformLocation(shaderProgramObject, "u_ld");
        lsUniform = glGetUniformLocation(shaderProgramObject, "u_ls");
        lightPositionUniform = glGetUniformLocation(shaderProgramObject, "u_lightPosition");

        kaUniform = glGetUniformLocation(shaderProgramObject, "u_ka");
        kdUniform = glGetUniformLocation(shaderProgramObject, "u_kd");
        ksUniform = glGetUniformLocation(shaderProgramObject, "u_ks");
        materialShinessUniform = glGetUniformLocation(shaderProgramObject, "u_materialShiness");
        alphaUniform = glGetUniformLocation(shaderProgramObject, "u_alpha");
        
        return TRUE;
    }

    void uninitialize(void)
    {
        UninitializeShaders(shaderProgramObject);
    }
};
