#pragma once

#include "../../utils/common.h"

class Model_ShaderParticleCommon
{

public:
    GLuint shaderProgramObject;

    GLuint projectMatrixUniformParticles; // model View Projection
    GLuint viewMatrixUniformParticles;    // model View Projection
    GLuint modelMatrixUniformParticles;   // model View Projection
    GLuint textureSamplerUniformParticles;
    GLuint alphaUniformParticles;

    BOOL initialize()
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\model\\modelParticles.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\model\\modelParticles.fs", FRAGMENT);

        shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // prelinked binding
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_POSITION, "a_position");
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_TEXTURE0, "a_texcoord");
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_COLOR, "a_color");

        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject);

        if (bShaderLinkStatus == FALSE)
            return FALSE;

        // post link - getting
        projectMatrixUniformParticles = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
        viewMatrixUniformParticles = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
        modelMatrixUniformParticles = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");
        textureSamplerUniformParticles = glGetUniformLocation(shaderProgramObject, "u_textureSampler");
        alphaUniformParticles = glGetUniformLocation(shaderProgramObject, "u_alpha");

        return TRUE;
    }

    void uninitialize(void)
    {
        if (shaderProgramObject)
        {
            UninitializeShaders(shaderProgramObject);
        }
    }
};


