#pragma once

// #include "../03_UtilityFiles/GLHeadersAndMacros.h"
// #include "../03_UtilityFiles/GLShaders.h"
// #include "../../utils/model/mesh.h"
#include "../../utils/model_using_assim/model.h"
#include "../../utils/common.h"
#include "../environmentMap/EnvironmentMap.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../utils/camera/Camera.h"

extern BezierCamera *globalBezierCamera;
extern Camera camera;

class ModelShader
{

public:
    // Variables
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

    Model *ourModel;
    // Disintegration effect configuration
    float particleSpeed = 0.05f;
    float dissolveTime = 0.0f;
    BOOL startDisintegration = FALSE;
    BOOL startIntegration = FALSE;
    int DISNTEGRATE_DIRECTION;
    float maxDisintegrateDistance = 3.0f;

    void initialize_ModelShaderObject(Model *loadedModel)
    {
        ourModel = loadedModel;
        ourModel->initializeMeshTextures();
        ourModel->initializeMesh();
    }

    BOOL initialize_ModelShaderObject(const char *path)
    {
        ourModel = new Model(path);
        return TRUE;
    }

    BOOL initialize_ModelShaderObject(const char *path, const char *textureFileName)
    {
        ourModel = new Model(path);
        return TRUE;
    }

    void render_Model(void)
    {

        if (ourModel->isDisintegration)
        {
            // Triggering the Particle shader
            if (startDisintegration || startIntegration)
                render_ModelParticles();
        }
        float matrix[3] = {objX, objY, objZ};

        // Use The Shader Program Object
        glUseProgram(commonShaders->modelShaderCommon->shaderProgramObject);

        static const GLfloat ones[] = {1.0f};

        if (isDOFTrue)
            glClearBufferfv(GL_DEPTH, 0, ones); // depth buffer clear

        // Transformations
        glUniformMatrix4fv(commonShaders->modelShaderCommon->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(commonShaders->modelShaderCommon->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(commonShaders->modelShaderCommon->projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

        glUniform3fv(commonShaders->modelShaderCommon->matrixUniform, 1, matrix);

        if (USE_FPV_CAM)
            glUniform3fv(commonShaders->modelShaderCommon->viewPosUniform, 1, camera.getEye());
        else
        {
            if (globalBezierCamera)
                glUniform3fv(commonShaders->modelShaderCommon->viewPosUniform, 1, globalBezierCamera->getCameraPosition());
        }

        // Start alpha if disintegration
        if (ourModel->isDisintegration)
        {
            alpha = lerp(dissolveTime, 1.0f, 0.0f);
        }
        // Return if object being drawn is blank
        if (alpha <= 0.0f)
            return;

        if (alpha < 1.0f)
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
        }

        glUniform1f(commonShaders->modelShaderCommon->alphaUniform, alpha);

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

        ourModel->Render(commonShaders->modelShaderCommon->shaderProgramObject);

        if (alpha < 1.0f)
        {
            glDisable(GL_BLEND);
        }
        glUseProgram(0);
    }

    void render_nargisModel(void)
    {

        if (ourModel->isDisintegration)
        {
            // Triggering the Particle shader
            if (startDisintegration || startIntegration)
                render_ModelParticles();
        }
        float matrix[3] = {objX, objY, objZ};

        // Use The Shader Program Object
        glUseProgram(commonShaders->modelShaderCommon->shaderProgramObject);

        static const GLfloat ones[] = {1.0f};

        if (isDOFTrue)
            glClearBufferfv(GL_DEPTH, 0, ones); // depth buffer clear

        // Transformations
        glUniformMatrix4fv(commonShaders->modelShaderCommon->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(commonShaders->modelShaderCommon->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(commonShaders->modelShaderCommon->projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

        glUniform3fv(commonShaders->modelShaderCommon->matrixUniform, 1, matrix);

        if (USE_FPV_CAM)
            glUniform3fv(commonShaders->modelShaderCommon->viewPosUniform, 1, camera.getEye());
        else
        {
            if (globalBezierCamera)
                glUniform3fv(commonShaders->modelShaderCommon->viewPosUniform, 1, globalBezierCamera->getCameraPosition());
        }

        // Start alpha if disintegration
        if (ourModel->isDisintegration)
        {
            alpha = lerp(dissolveTime, 1.0f, 0.0f);
        }
        // Return if object being drawn is blank
        if (alpha <= 0.0f)
            return;

        if (alpha < 1.0f)
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
        }

        static float nargisAlpha = 1.0f;
        if (ELAPSED_TIME >= 230 && PHASE_CURRENT == PHASE_MAIN || barsaatNargisFadeoutTrigger == TRUE)
        {
            barsaatNargisFadeoutTrigger = TRUE;
            if (nargisAlpha > 0.0f)
            {
                nargisAlpha = nargisAlpha - 0.01f;
                if (nargisAlpha < 0.3f)
                    barsaatSceneDupattaFallTrigger = TRUE;
            }
        }
        //setAlpha(nargisAlpha);

        glUniform1f(commonShaders->modelShaderCommon->alphaUniform, nargisAlpha);

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

        ourModel->Render(commonShaders->modelShaderCommon->shaderProgramObject);

        if (alpha < 1.0f)
        {
            glDisable(GL_BLEND);
        }
        glUseProgram(0);
    }

    void render_ModelParticles(void)
    {
        pushMatrix(modelMatrix);
        {
            // Use The Shader Program Object
            glUseProgram(commonShaders->particleShader->shaderProgramObject);

            // Transformations
            glUniformMatrix4fv(commonShaders->particleShader->modelMatrixUniformParticles, 1, GL_FALSE, modelMatrix);
            glUniformMatrix4fv(commonShaders->particleShader->viewMatrixUniformParticles, 1, GL_FALSE, viewMatrix);
            glUniformMatrix4fv(commonShaders->particleShader->projectMatrixUniformParticles, 1, GL_FALSE, perspectiveProjectionMatrix);

            glUniform1i(commonShaders->particleShader->textureSamplerUniformParticles, 0);
            if (startDisintegration)
                glUniform1f(commonShaders->particleShader->alphaUniformParticles, dissolveTime * 1.5f);
            else
                glUniform1f(commonShaders->particleShader->alphaUniformParticles, dissolveTime);

            ourModel->RenderParticles(particleSpeed, startDisintegration, startIntegration, DISNTEGRATE_DIRECTION, this->maxDisintegrateDistance);

            // Unuse The Shader Program Object
            glUseProgram(0);
        }
        modelMatrix = popMatrix();
    }

    void render_Models(const int numberOfInstances, float *instancesPositionArray, BOOL isUP)
    {
        GLfloat eqn[] = {0.0f, -1.0f, 0.0f, 0.0f};
        if (isUP == TRUE)
        {
            eqn[1] = 1.0f;
            eqn[3] = 0.0f;
        }
        else
        {
            eqn[1] = -1.0f;
            eqn[3] = 0.0f;
        }

        // Use The Shader Program Object
        glUseProgram(commonShaders->modelShaderCommon->shaderProgramObject);

        // Transformations
        glUniformMatrix4fv(commonShaders->modelShaderCommon->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(commonShaders->modelShaderCommon->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(commonShaders->modelShaderCommon->projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture);

        glUniform1i(commonShaders->modelShaderCommon->textureSamplerUniform, 0);

        glUniform3fv(commonShaders->modelShaderCommon->matrixUniform, numberOfInstances, instancesPositionArray);

        glUniform4fv(commonShaders->modelShaderCommon->clipPlaneUniform, 1, eqn);

        // Sending Light Related Uniforms
        if (bLight == TRUE)
        {
            glUniform1i(commonShaders->modelShaderCommon->lightingEnabledUniform_PF, 1);

            glUniform3fv(commonShaders->modelShaderCommon->laUniform_PF, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
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

        // glBindVertexArray(vao_Model);

        // glBindBuffer(GL_ARRAY_BUFFER, vbo_Matrices);
        // glBufferData(GL_ARRAY_BUFFER, 9.0f * 16.0f * sizeof(float), matrices, GL_DYNAMIC_DRAW);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // glDrawArrays(GL_TRIANGLES, 0, mesh_data->gp_vertex_filter->size);
        // glDrawArraysInstanced(GL_TRIANGLES, 0, model.vertecesAfterFilter.size(), numberOfInstances);

        // glBindVertexArray(0);

        // Unuse The Shader Program Object
        glUseProgram(0);
    }

    void render_Models1(const int numberOfInstances, mat4 matrices)
    {

        // float matrix[6] = {-2.0, 0.0, -2.0,
        //                    2.0, 0.0, 0.0};

        lightPositions[0] = eye[0];
        lightPositions[1] = eye[1];
        lightPositions[2] = eye[2];

        // Use The Shader Program Object
        glUseProgram(commonShaders->modelShaderCommon->shaderProgramObject);

        // Transformations
        glUniformMatrix4fv(commonShaders->modelShaderCommon->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(commonShaders->modelShaderCommon->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(commonShaders->modelShaderCommon->projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture);
        // glUniform1i(commonShaders->modelShaderCommon->textureSamplerUniform, 0);

        glUniformMatrix4fv(commonShaders->modelShaderCommon->matrixUniform, numberOfInstances, GL_FALSE, matrices[0]);

        // Sending Light Related Uniforms
        if (bLight == TRUE)
        {
            glUniform1i(commonShaders->modelShaderCommon->lightingEnabledUniform_PF, 1);

            glUniform3fv(commonShaders->modelShaderCommon->laUniform_PF, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
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

        // glBindVertexArray(vao_Model);

        // glBindBuffer(GL_ARRAY_BUFFER, vbo_Matrices);
        // glBufferData(GL_ARRAY_BUFFER, 9.0f * 16.0f * sizeof(float), matrices, GL_DYNAMIC_DRAW);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // glDrawArrays(GL_TRIANGLES, 0, mesh_data->gp_vertex_filter->size);
        // glDrawArraysInstanced(GL_TRIANGLES, 0, model.vertecesAfterFilter.size(), numberOfInstances);

        // glBindVertexArray(0);

        // Unuse The Shader Program Object
        glUseProgram(0);
    }

    void setAlpha(float _alpha)
    {
        alpha = _alpha;
    }

    // Rendering colored model
    void render_Model_Colored(ColorShader *shader)
    {

        // float matrix[3] = {objX, objY, objZ};

        // lightPositions[0] = eye[0];
        // lightPositions[1] = eye[1];
        // lightPositions[2] = eye[2];

        // Use The Shader Program Object
        glUseProgram(shader->shaderProgramObject);

        // Transformations
        glUniformMatrix4fv(shader->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(shader->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(shader->projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

        // glUniform3fv(matrixUniform, 1, matrix);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // glBindVertexArray(vao_Model);
        // glDrawArrays(GL_TRIANGLES, 0, mesh_data->gp_vertex_filter->size);
        // glDrawArraysInstanced(GL_TRIANGLES, 0, model.vertecesAfterFilter.size(), 1);
        // glBindVertexArray(0);
        ourModel->Render(shader->shaderProgramObject);
        // Unuse The Shader Program Object
        glUseProgram(0);
    }

    void uninitialize_ModelShaderObject(void)
    {

        // if (ourModel)
        // {
        //     delete (ourModel);
        //     ourModel = NULL;
        // }
    }

    void disintegrate(int direction, float modelFadeoutTime, float particleSpeed, float maxDistance)
    {
        // Setting up flags
        this->startDisintegration = TRUE;
        this->startIntegration = FALSE;

        // Setting values
        this->dissolveTime = modelFadeoutTime;
        this->particleSpeed = particleSpeed;
        this->DISNTEGRATE_DIRECTION = direction;
        this->maxDisintegrateDistance = maxDistance;
    }
    void integrate(int direction, float modelFadeinTime, float particleSpeed, float maxDistance)
    {
        // Setting up flags
        this->startDisintegration = FALSE;
        this->startIntegration = TRUE;

        // Setting values
        this->dissolveTime = modelFadeinTime;
        this->particleSpeed = particleSpeed;
        this->DISNTEGRATE_DIRECTION = direction;
        this->maxDisintegrateDistance = maxDistance;
    }
    void stopDisintegration()
    {
        // Setting up flags
        this->startDisintegration = FALSE;
        this->startIntegration = FALSE;
    }
    static float lerp(float t, float a, float b)
    {
        return a + t * (b - a);
    }

    // void render_Model_KtxTexture(EnvMapShader newEnvMap)
    // {

    // float matrix[3] = {objX, objY, objZ};

    // // Use The Shader Program Object
    // glUseProgram(newEnvMap.shaderProgramObject_model_chromeMap);

    // // Transformations
    // glUniformMatrix4fv(newEnvMap.modelMatrixUniform_chromeMap, 1, GL_FALSE, modelMatrix);
    // glUniformMatrix4fv(newEnvMap.viewMatrixUniform_chromeMap, 1, GL_FALSE, viewMatrix);
    // glUniformMatrix4fv(newEnvMap.projectionMatrixUniform_chromeMap, 1, GL_FALSE, perspectiveProjectionMatrix);

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, newEnvMap.ktxTexture_chromeMap3_Chrome);
    // glUniform1i(textureSamplerUniform, 0);

    // //// Sending Light Related Uniforms
    // // if (bLight == TRUE)
    // //{

    // //    glUniform1i(lightingEnabledUniform_PF, 1);

    // //    glUniform3fv(laUniform_PF, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
    // //    glUniform3fv(ldUniform_PF, 1, lightDiffuse);
    // //    glUniform3fv(lsUniform_PF, 1, lightSpecular);
    // //    glUniform4fv(lighPositionUniform_PF, 1, lightPositions);

    // //    glUniform3fv(kaUniform_PF, 1, materialAmbiant);
    // //    glUniform3fv(kdUniform_PF, 1, meterialDeffuse);
    // //    glUniform3fv(ksUniform_PF, 1, materialSpecular);
    // //    glUniform1f(materialShininessUniform_PF, materialShineeness);
    // //}
    // // else
    // //{
    // //    glUniform1i(lightingEnabledUniform_PF, 0);
    // //}

    // ourModel->Render_Ktx(shaderProgramObject_model);

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, 0);

    // glUseProgram(0);
    // }
};
