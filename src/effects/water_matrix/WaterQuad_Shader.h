#pragma once

#include "../../utils/common.h"
#include "../../utils/camera/Camera.h"

class WaterQuadShader
{
public:
    // Variables
    // Water Quad
    GLuint shaderProgramObject_waterQuad;

    GLuint modelMatrixUniform_waterQuad;
    GLuint viewMatrixUniform_waterQuad;
    GLuint projectionMatrixUniform_waterQuad;

    GLuint reflectionTextureSamplerUniform_waterQuad;
    GLuint refractionTextureSamplerUniform_waterQuad;

    GLuint waterDUDVMapTextureSamplerUniform_waterQuad;
    GLuint waterNormalMapTextureSamplerUniform_waterQuad;

    GLuint moveFactorOffsetUniform_waterQuad;

    GLuint lightColorUniform_waterQuad;
    GLuint lightPositionUniform_waterQuad;

    GLuint cameraPositionUniform_waterQuad;

    GLuint resolutionUniform;

    GLuint vignetteOuterRadiusUniform;
    GLuint vignetteInnerRadiusUniform;

    GLuint waterColor_JisDesh_Uniform;
    GLuint interpolateDarkToBright_uniform;

    BOOL initialize(void)
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\water_matrix\\WaterQuad.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\water_matrix\\WaterQuad.fs", FRAGMENT);

        shaderProgramObject_waterQuad = glCreateProgram();

        // Step 2
        glAttachShader(shaderProgramObject_waterQuad, vertexShaderObject);
        glAttachShader(shaderProgramObject_waterQuad, fragmentShaderObject);

        // Pre-Linking Binding Of Vertex Attribute Locations
        glBindAttribLocation(shaderProgramObject_waterQuad, MATRIX_ATTRIBUTE_POSITION, "a_position");
        glBindAttribLocation(shaderProgramObject_waterQuad, MATRIX_ATTRIBUTE_COLOR, "a_color");

        if (LinkShaderProgramObject(shaderProgramObject_waterQuad) == FALSE)
        {
            PrintLog("LinkShaderProgramObject() Failed.\n");
            return (FALSE);
        }

        // Post-Linking Getting Of Uniform Locations
        modelMatrixUniform_waterQuad = glGetUniformLocation(shaderProgramObject_waterQuad, "u_modelMatrix");
        viewMatrixUniform_waterQuad = glGetUniformLocation(shaderProgramObject_waterQuad, "u_viewMatrix");
        projectionMatrixUniform_waterQuad = glGetUniformLocation(shaderProgramObject_waterQuad, "u_projectionMatrix");

        reflectionTextureSamplerUniform_waterQuad = glGetUniformLocation(shaderProgramObject_waterQuad, "u_reflectionTextureSampler");
        refractionTextureSamplerUniform_waterQuad = glGetUniformLocation(shaderProgramObject_waterQuad, "u_refractionTextureSampler");
        waterDUDVMapTextureSamplerUniform_waterQuad = glGetUniformLocation(shaderProgramObject_waterQuad, "u_waterDUDVMapTextureSampler");
        waterNormalMapTextureSamplerUniform_waterQuad = glGetUniformLocation(shaderProgramObject_waterQuad, "u_waterNormalMapTextureSampler");

        moveFactorOffsetUniform_waterQuad = glGetUniformLocation(shaderProgramObject_waterQuad, "u_moveFactorOffset");
        cameraPositionUniform_waterQuad = glGetUniformLocation(shaderProgramObject_waterQuad, "u_cameraPosition");
        lightPositionUniform_waterQuad = glGetUniformLocation(shaderProgramObject_waterQuad, "u_lightPosition");
        lightColorUniform_waterQuad = glGetUniformLocation(shaderProgramObject_waterQuad, "u_lightColor");

        resolutionUniform = glGetUniformLocation(shaderProgramObject_waterQuad, "u_resolution");

        vignetteOuterRadiusUniform = glGetUniformLocation(shaderProgramObject_waterQuad, "u_vignetteOuterRadius");
        vignetteInnerRadiusUniform = glGetUniformLocation(shaderProgramObject_waterQuad, "u_vignetteInnerRadius");

        waterColor_JisDesh_Uniform = glGetUniformLocation(shaderProgramObject_waterQuad, "waterColor_JisDesh");
        interpolateDarkToBright_uniform = glGetUniformLocation(shaderProgramObject_waterQuad, "interpolateDarkToBright");

        return TRUE;
    }

    void update(void)
    {
    }

    void uninitialize(void)
    {
        UninitializeShaders(shaderProgramObject_waterQuad);
    }
};