#pragma once
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLShaders.h"

class CubemapShaderMerge
{
public:
    // Variables
    GLuint shaderProgramObject;
    GLuint projectionMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint modelMatrixUniform;
    GLuint skyBoxUniform1;
    GLuint skyBoxUniform2;
    GLuint integrateFactorUniform;
    // Member Functions
    BOOL initialize()
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\cubemapMerge\\cubemap.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\cubemapMerge\\cubemap.fs", FRAGMENT);

        shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // prelinked binding
        // Binding Position Array
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_POSITION, "a_position");
        // Binding Color Array
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_COLOR, "a_color");

        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject);

        if (bShaderLinkStatus == FALSE)
            return FALSE;

        // post link - getting
        projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
        viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
        modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");
        skyBoxUniform1 = glGetUniformLocation(shaderProgramObject, "u_skyBox1");
        skyBoxUniform2 = glGetUniformLocation(shaderProgramObject, "u_skyBox2");
        integrateFactorUniform = glGetUniformLocation(shaderProgramObject, "u_factor");

        return TRUE;
    }

    void uninitialize(void)
    {
        UninitializeShaders(shaderProgramObject);
    }
};
