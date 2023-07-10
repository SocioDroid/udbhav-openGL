#pragma once
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLShaders.h"

class VolumeCloudShader
{
public:
    // Variables
    GLuint shaderProgramObject;
    
    // Vertex Shader
    GLuint projectionMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint modelMatrixUniform;
    GLuint cameraPositionUniform;

    // Fragment Shader
    GLuint baseUniform;
    GLuint mapUniform;
    GLuint thresholdUniform;
    GLuint rangeUniform;
    GLuint opacityUniform;
    GLuint stepsUniform;
    GLuint frameUniform;
    GLuint randomNumUniform;

    // Member Functions
    BOOL initialize()
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\volumeCloud\\cloud.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\volumeCloud\\cloud.fs", FRAGMENT);

        shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // prelinked binding
        // Binding Position Array
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_POSITION, "a_position");

        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject);

        if (bShaderLinkStatus == FALSE)
            return FALSE;

        // post link - getting
        projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
        viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
        modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");
        cameraPositionUniform = glGetUniformLocation(shaderProgramObject, "u_cameraPos");

        baseUniform = glGetUniformLocation(shaderProgramObject, "u_base");
        mapUniform = glGetUniformLocation(shaderProgramObject, "u_map");
        thresholdUniform = glGetUniformLocation(shaderProgramObject, "u_threshold");
        rangeUniform = glGetUniformLocation(shaderProgramObject, "u_range");
        opacityUniform = glGetUniformLocation(shaderProgramObject, "u_opacity");
        stepsUniform = glGetUniformLocation(shaderProgramObject, "u_steps");
        frameUniform = glGetUniformLocation(shaderProgramObject, "u_frame");
        randomNumUniform = glGetUniformLocation(shaderProgramObject, "u_randomNum");

        return TRUE;
    }

    void uninitialize(void)
    {
        UninitializeShaders(shaderProgramObject);
    }
};
