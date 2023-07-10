#pragma once

#include "../../utils/model_using_assim/model.h"
#include "../../utils/common.h"
#include "../environmentMap/EnvironmentMap.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../utils/camera/Camera.h"

extern BezierCamera *globalBezierCamera;
extern Camera camera;

class ModelShaderDyanamic
{

public:
    // Variables
    BOOL bLight = TRUE;

    GLfloat lightAmbiant[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPositions[4] = {0.0f, 100.0f, 100.0f, 1.0f};

    GLfloat materialAmbiant[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat meterialDeffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat materialSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat materialShineeness = 100.0f;
    float alpha = 1.0f;
    float fadealpha = 1.0f;

    Model ourModel;
    // Disintegration effect configuration
    float particleSpeed = 0.05f;
    float dissolveTime = 0.0f;
    BOOL startDisintegration = FALSE;
    BOOL startIntegration = FALSE;
    int DISNTEGRATE_DIRECTION;
    float maxDisintegrateDistance = 3.0f;

    void initialize_ModelShaderObject(Model *loadedModel)
    {
        // ourModel = loadedModel;
        // ourModel->initializeMeshTextures();
        // ourModel->initializeMesh();
    }

    BOOL initialize_ModelShaderObject(const char *path)
    {
        // ourModel = new Model(path);
        // return TRUE;
    }

    BOOL initialize_ModelShaderObject(const char *path, const char *textureFileName)
    {
        // ourModel = new Model(path);
        // return TRUE;
    }

    void render_Model()
    {

        // Use The Shader Program Object
        glUseProgram(commonShaders->modelShaderCommon->shaderProgramObject);

        static const GLfloat ones[] = {1.0f};

        if (isDOFTrue)
            glClearBufferfv(GL_DEPTH, 0, ones); // depth buffer clear

        // Transformations
        glUniformMatrix4fv(commonShaders->modelShaderCommon->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(commonShaders->modelShaderCommon->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(commonShaders->modelShaderCommon->projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

        // glUniform3fv(commonShaders->modelShaderCommon->matrixUniform, 1, matrix);

        if (USE_FPV_CAM)
            glUniform3fv(commonShaders->modelShaderCommon->viewPosUniform, 1, camera.getEye());
        else
        {
            if (globalBezierCamera)
                glUniform3fv(commonShaders->modelShaderCommon->viewPosUniform, 1, globalBezierCamera->getCameraPosition());
        }

        glUniform1f(commonShaders->modelShaderCommon->alphaUniform, alpha);
        glUniform1f(commonShaders->modelShaderCommon->fadeoutAlphaUniform, fadealpha);

        // glUniform4fv(clipPlaneUniform, 1, eqn);
        // Sending Light Related Uniforms
        if (bLight == TRUE)
        {

            glUniform1i(commonShaders->modelShaderCommon->lightingEnabledUniform_PF, 1);

            glUniform3fv(commonShaders->modelShaderCommon->laUniform_PF, 1, lightAmbiant);
            glUniform3fv(commonShaders->modelShaderCommon->ldUniform_PF, 1, lightDiffuse);
            glUniform3fv(commonShaders->modelShaderCommon->lsUniform_PF, 1, lightSpecular);
            glUniform4fv(commonShaders->modelShaderCommon->lighPositionUniform_PF, 1, lightPositions);

            glUniform3fv(commonShaders->modelShaderCommon->kaUniform_PF, 1, materialAmbiant);
            glUniform3fv(commonShaders->modelShaderCommon->kdUniform_PF, 1, meterialDeffuse);
            glUniform3fv(commonShaders->modelShaderCommon->ksUniform_PF, 1, materialSpecular);
            glUniform1f(commonShaders->modelShaderCommon->materialShininessUniform_PF, materialShineeness);
        }
        else
        {
            glUniform1i(commonShaders->modelShaderCommon->lightingEnabledUniform_PF, 0);
        }

        // loadedModel->Render(commonShaders->modelShaderCommon->shaderProgramObject);
    }

    void setAlpha(float _alpha)
    {
        alpha = _alpha;
    }

    void uninitialize_ModelShaderObject(void)
    {
    }
};
