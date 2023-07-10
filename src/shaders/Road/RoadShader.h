#pragma once
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLShaders.h"

class RoadShader
{
public:
    // Variables
    /* SKY RELATED VARIALBES*/
    GLuint shaderProgramObject;

    GLuint projectionMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint modelMatrixUniform;
    GLuint textureSamplerUniform;
    GLuint normalTextureSamplerUniform;

    GLuint laUniform; // Light ambiwnt
    GLuint ldUniform; // Light diffuse 
    GLuint lsUniform; // Light specular
    GLuint lightPositionUniform; 

    GLuint kaUniform; // Material ambient
    GLuint kdUniform; // Material diffuse
    GLuint ksUniform; // Material specular
    GLuint materialShininessUniform; 

    BOOL initialize(void)
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects("./src/shaders/Road/road.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects("./src/shaders/Road/road.fs", FRAGMENT);

        shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // prelinked binding
        // Binding Position Array
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_POSITION, "a_position");
        // Binding Color Array
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_TEXTURE0, "a_texcoord");
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_NORMAL, "a_normal");

        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject);

        if (bShaderLinkStatus == FALSE)
            return FALSE;

        // post link - getting
        projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
        viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
        modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");

        textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");
        normalTextureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureNormalSampler");

        laUniform = glGetUniformLocation(shaderProgramObject, "u_la");
	    ldUniform = glGetUniformLocation(shaderProgramObject, "u_ld");
	    lsUniform = glGetUniformLocation(shaderProgramObject, "u_ls");
	    lightPositionUniform = glGetUniformLocation(shaderProgramObject, "u_lightPosition");
    
	    kaUniform = glGetUniformLocation(shaderProgramObject, "u_ka");
	    kdUniform = glGetUniformLocation(shaderProgramObject, "u_kd");
	    ksUniform = glGetUniformLocation(shaderProgramObject, "u_ks");
	    materialShininessUniform = glGetUniformLocation(shaderProgramObject, "u_materialShininess");

        return TRUE;
    }

    void uninitialize(void)
    {
        UninitializeShaders(shaderProgramObject);
    }
};