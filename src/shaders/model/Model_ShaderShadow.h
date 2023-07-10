#pragma once

#include "../../utils/model_using_assim/model.h"
#include "../../utils/common.h"
extern Camera camera;

class ModelShaderShadow
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

    GLuint lightSpaceMatrixUniform;
    GLuint shadowMapTextureUniform_shadow;

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

    GLuint lightPosUniform_shadow;
    GLuint viewPosUniform_shadow;
    GLuint disableShadowUniform_shadow;
    GLuint alphaUniform_shadow;

    Model *ourModel;

    GLuint SHADOW_WIDTH = 1024;
    GLuint SHADOW_HEIGHT = 1024;
    int disableShadow = 0;
    float alpha = 1.0f;

    // Disintegration effect configuration
    float particleSpeed = 0.05f;
    float dissolveTime = 0.0f;
    BOOL startDisintegration = FALSE;
    BOOL startIntegration = FALSE;
    int DISNTEGRATE_DIRECTION;
    float maxDisintegrateDistance = 3.0f;

    BOOL initialize_ModelShaderObject(const char *path)
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\model\\model_shadow.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\model\\model_shadow.fs", FRAGMENT);

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

        lightSpaceMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_lightSpaceMatrix");
        shadowMapTextureUniform_shadow = glGetUniformLocation(shaderProgramObject, "shadowMap");

        lightPosUniform_shadow = glGetUniformLocation(shaderProgramObject, "lightPos");
        viewPosUniform_shadow = glGetUniformLocation(shaderProgramObject, "viewPos");
        disableShadowUniform_shadow = glGetUniformLocation(shaderProgramObject, "disableShadow");
        alphaUniform_shadow = glGetUniformLocation(shaderProgramObject, "u_alpha");

        ourModel = new Model(path);
        ourModel->initializeMeshTextures();
        ourModel->initializeMesh();

        // if (LoadPNGImage(&textureNormal, "./src/04_Assets/03_3DModels/Fern/NormalMap.png") == FALSE)
        //{
        //     PrintLog("LoadPNGImage Failed...\n");
        //     // return FALSE;
        // }

        return TRUE;
    }

    BOOL initialize_ModelShaderObject(Model *model)
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\model\\model_shadow.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\model\\model_shadow.fs", FRAGMENT);

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

        lightSpaceMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_lightSpaceMatrix");
        shadowMapTextureUniform_shadow = glGetUniformLocation(shaderProgramObject, "shadowMap");

        lightPosUniform_shadow = glGetUniformLocation(shaderProgramObject, "lightPos");
        viewPosUniform_shadow = glGetUniformLocation(shaderProgramObject, "viewPos");
        disableShadowUniform_shadow = glGetUniformLocation(shaderProgramObject, "disableShadow");

        ourModel = model;
        ourModel->initializeMeshTextures();
        ourModel->initializeMesh();

        // if (LoadPNGImage(&textureNormal, "./src/04_Assets/03_3DModels/Fern/NormalMap.png") == FALSE)
        //{
        //     PrintLog("LoadPNGImage Failed...\n");
        //     // return FALSE;
        // }

        return TRUE;
    }

    void update_ModelShaderObjectUniforms(void)
    {
    }

    void render_Model(GLuint _shaderProgramObject)
    {
    }

    void render_Model_shadow(GLuint _shaderProgramObject, BOOL isDepthMap, GLuint depthMapTexture, mat4 lightSpaceMatrix, vec3 lightPos)
    {

        if (ourModel->isDisintegration)
        {
            // Triggering the Particle shader
            if (startDisintegration || startIntegration)
                render_ModelParticles();
        }

        mat4 lightProjectionMatrix = mat4::identity();
        mat4 lightViewMatrix = mat4::identity();
        float near_plane = 1.0f, far_plane = 7.5f;

        if (isDepthMap == FALSE)
        {
            // GLfloat eqn[] = {0.0f, -1.0f, 0.0f, 0.0f};
            // if (isUP == TRUE)
            // {
            //     eqn[1] = 1.0f;
            //     // eqn[3] = -8.5f;
            //     eqn[3] = 0.0f;
            // }
            // else
            // {
            //     eqn[1] = 1.0f;
            //     // eqn[3] = 8.5f;
            //     eqn[3] = 0.0f;
            // }

            float matrix[3] = {objX, objY, objZ};

            // lightPositions[0] = eye[0];
            // lightPositions[1] = eye[1];
            // lightPositions[2] = eye[2];

            // Use The Shader Program Object
            glUseProgram(shaderProgramObject);

            // Transformations
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
            glUniformMatrix4fv(projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            glUniformMatrix4fv(lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);
            glUniform3fv(matrixUniform, 1, matrix);
            glUniform1i(disableShadowUniform_shadow, disableShadow);
            // glUniform4fv(clipPlaneUniform, 1, eqn);

            // glActiveTexture(GL_TEXTURE0);
            // glBindTexture(GL_TEXTURE_2D, texture);
            // glUniform1i(textureSamplerUniform, 0);

            // glActiveTexture(GL_TEXTURE1);
            // glBindTexture(GL_TEXTURE_2D, textureNormal);
            // glUniform1i(textureSamplerUniform, 1);

            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, depthMapTexture);
            glUniform1i(shadowMapTextureUniform_shadow, 4);

            vec3 viewPos;
            if (USE_FPV_CAM)
            {
                viewPos = camera.getEye();
            }
            else
            {
                if (globalBezierCamera)
                    viewPos = globalBezierCamera->getEye();
                else
                    viewPos = camera.getEye();
            }

            glUniform3fv(viewPosUniform_shadow, 1, viewPos);
            glUniform3fv(lightPosUniform_shadow, 1, lightPos);

            // Sending Light Related Uniforms
            if (bLight == TRUE)
            {

                glUniform1i(lightingEnabledUniform_PF, 1);

                glUniform3fv(laUniform_PF, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
                glUniform3fv(ldUniform_PF, 1, lightDiffuse);
                glUniform3fv(lsUniform_PF, 1, lightSpecular);
                glUniform4fv(lighPositionUniform_PF, 1, lightPositions);

                glUniform3fv(kaUniform_PF, 1, materialAmbiant);
                glUniform3fv(kdUniform_PF, 1, meterialDeffuse);
                glUniform3fv(ksUniform_PF, 1, materialSpecular);
                glUniform1f(materialShininessUniform_PF, materialShineeness);
            }
            else
            {
                glUniform1i(lightingEnabledUniform_PF, 0);
            }

            // Start alpha if disintegration
            if (ourModel->isDisintegration)
            {
                alpha = lerp(dissolveTime, 1.0f, 0.0f);
            }

            if (alpha < 1.0f)
            {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glEnable(GL_BLEND);
            }
            glUniform1f(alphaUniform_shadow, alpha);

            ourModel->Render(shaderProgramObject);

            if (alpha < 1.0f)
            {
                glDisable(GL_BLEND);
            }
            glUseProgram(0);
        }
        else
        {

            ourModel->Render(_shaderProgramObject);
        }
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

        // float matrix[6] = {-2.0, 0.0, -2.0,
        //                    2.0, 0.0, 0.0};

        // lightPositions[0] = eye[0];
        // lightPositions[1] = eye[1];
        // lightPositions[2] = eye[2];

        // Use The Shader Program Object
        glUseProgram(shaderProgramObject);

        // Transformations
        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(textureSamplerUniform, 0);

        glUniform3fv(matrixUniform, numberOfInstances, instancesPositionArray);

        glUniform4fv(clipPlaneUniform, 1, eqn);

        // Sending Light Related Uniforms
        if (bLight == TRUE)
        {
            glUniform1i(lightingEnabledUniform_PF, 1);

            glUniform3fv(laUniform_PF, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
            glUniform3fv(ldUniform_PF, 1, lightDiffuse);
            glUniform3fv(lsUniform_PF, 1, lightSpecular);
            glUniform4fv(lighPositionUniform_PF, 1, lightPositions);

            glUniform3fv(kaUniform_PF, 1, materialAmbiant);
            glUniform3fv(kdUniform_PF, 1, meterialDeffuse);
            glUniform3fv(ksUniform_PF, 1, materialSpecular);
            glUniform1f(materialShininessUniform_PF, materialShineeness);
        }
        else
        {
            glUniform1i(lightingEnabledUniform_PF, 0);
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
        glUseProgram(shaderProgramObject);

        // Transformations
        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(textureSamplerUniform, 0);

        glUniformMatrix4fv(matrixUniform, numberOfInstances, GL_FALSE, matrices[0]);

        // Sending Light Related Uniforms
        if (bLight == TRUE)
        {
            glUniform1i(lightingEnabledUniform_PF, 1);

            glUniform3fv(laUniform_PF, 1, lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
            glUniform3fv(ldUniform_PF, 1, lightDiffuse);
            glUniform3fv(lsUniform_PF, 1, lightSpecular);
            glUniform4fv(lighPositionUniform_PF, 1, lightPositions);

            glUniform3fv(kaUniform_PF, 1, materialAmbiant);
            glUniform3fv(kdUniform_PF, 1, meterialDeffuse);
            glUniform3fv(ksUniform_PF, 1, materialSpecular);
            glUniform1f(materialShininessUniform_PF, materialShineeness);
        }
        else
        {
            glUniform1i(lightingEnabledUniform_PF, 0);
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

    void uninitialize_ModelShaderObject(void)
    {
        if (ourModel)
        {
            delete (ourModel);
            ourModel = NULL;
        }

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
