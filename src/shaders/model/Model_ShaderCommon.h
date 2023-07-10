#pragma once

// #include "../03_UtilityFiles/GLHeadersAndMacros.h"
// #include "../03_UtilityFiles/GLShaders.h"
// #include "../../utils/model/mesh.h"
#include "../../utils/model_using_assim/model.h"
#include "../../utils/common.h"

#include "../environmentMap/EnvironmentMap.h"

class ModelShaderCommon
{

public:
    // Variables
    GLuint shaderProgramObject;

    GLuint projectMatrixUniform; // model View Projection
    GLuint viewMatrixUniform;    // model View Projection
    GLuint modelMatrixUniform;   // model View Projection

    GLuint vao_Model;
    GLuint vbo_Model_Position;
    GLuint vbo_Model_Normal;
    GLuint vbo_Model_Texcoord;
    GLuint vbo_Matrices;

    GLuint laUniform_PF;           // light Ambiant
    GLuint ldUniform_PF;           // light Diffuse
    GLuint lsUniform_PF;           // light Spec
    GLuint lighPositionUniform_PF; // light Position

    GLuint kaUniform_PF; // material amb
    GLuint kdUniform_PF; // mat diff
    GLuint ksUniform_PF; // mat specular
    GLuint materialShininessUniform_PF;

    GLuint lightingEnabledUniform_PF;

    GLuint textureSamplerUniform;
    GLuint normalSamplerUniform;
    GLuint matrixUniform;
    GLuint clipPlaneUniform;
    GLuint viewPosUniform;
    GLuint alphaUniform;
    GLuint fadeoutAlphaUniform;

    GLuint texture;
    GLuint textureNormal;
    BOOL bLight = TRUE;

    GLfloat lightAmbiant[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPositions[4] = {0.0f, 100.0f, 100.0f, 1.0f};

    GLfloat materialAmbiant[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat meterialDeffuse[4] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat materialSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat materialShineeness = 20.0f;

    float alpha = 1.0f;
    float alphaFadeOutAlpha = 1.0f;

    BOOL initialize()
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\model\\model.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\model\\model.fs", FRAGMENT);

        shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // prelinked binding
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_POSITION, "a_position");
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_NORMAL, "a_normal");
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_TEXTURE0, "a_texcoord");
        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject);

        if (bShaderLinkStatus == FALSE)
            return FALSE;

        // post link - getting
        projectMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
        viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
        modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");

        laUniform_PF = glGetUniformLocation(shaderProgramObject, "u_la");
        ldUniform_PF = glGetUniformLocation(shaderProgramObject, "u_ld");
        lsUniform_PF = glGetUniformLocation(shaderProgramObject, "u_ls");
        lighPositionUniform_PF = glGetUniformLocation(shaderProgramObject, "u_lightPosition");

        kaUniform_PF = glGetUniformLocation(shaderProgramObject, "u_ka");
        kdUniform_PF = glGetUniformLocation(shaderProgramObject, "u_kd");
        ksUniform_PF = glGetUniformLocation(shaderProgramObject, "u_ks");

        materialShininessUniform_PF = glGetUniformLocation(shaderProgramObject, "u_materialShininnes");

        lightingEnabledUniform_PF = glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");

        textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");
        normalSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_normalMap");
        matrixUniform = glGetUniformLocation(shaderProgramObject, "offsets");
        clipPlaneUniform = glGetUniformLocation(shaderProgramObject, "u_plane");

        viewPosUniform = glGetUniformLocation(shaderProgramObject, "viewPos");
        alphaUniform = glGetUniformLocation(shaderProgramObject, "u_alpha");
        fadeoutAlphaUniform = glGetUniformLocation(shaderProgramObject, "u_fade_alpha");

        return TRUE;
    }

    void uninitialize_ModelShaderObject(void)
    {

        if (texture)
        {
            glDeleteTextures(1, &texture);
            texture = 0;
        }

        if (shaderProgramObject)
        {
            UninitializeShaders(shaderProgramObject);
        }
    }
};
