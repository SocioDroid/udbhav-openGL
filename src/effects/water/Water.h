#pragma once
#include "../../utils/common.h"
#include "../../utils/camera/Camera.h"
#include "../../shaders/water/WaterShader.h"
#include "buffers.h"

extern Camera camera;

class Water
{
public:
    const int res = 2;
    Water(vec2 position, float scale, float height)
    {
        this->scale = scale;
        this->height = height;

        shad = new WaterShader();
        if (!shad->initialize())
        {
            PrintLog("Water Shader Initialization Failed\n");
        }

        vmath::mat4 identity;
        vmath::mat4 scaleMatrix = vmath::scale(scale, scale, scale);
        vmath::mat4 transMatrix = vmath::translate(position[0], height, position[1]);
        this->modelMatrix = transMatrix * scaleMatrix;

        reflectionFBO = new FrameBufferObject(FBOw, FBOh);
        refractionFBO = new FrameBufferObject(FBOw, FBOh);

        initializePlaneVAO(2, 10. * 100., &planeVAO, &planeVBO, &planeEBO);
    }

    void bindRefractionFBO()
    {
        reflectionFBO->bind();
    }

    void bindReflectionFBO()
    {
        refractionFBO->bind();
    }
    void drawVertices()
    {
        glBindVertexArray(planeVAO);
        glUseProgram(shad->shaderProgramObject);
        glDrawElements(GL_TRIANGLES, (res - 1) * (res - 1) * 2 * 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void unbindFBO()
    {
        unbindCurrentFrameBuffer(giWindowWidth, giWindowHeight);
    }

    void setPosition(vec2 position, float scale, float height)
    {
        mat4 identity;
        mat4 scaleMatrix = vmath::scale(scale, scale, scale);
        mat4 transMatrix = vmath::translate(position[0], height, position[1]);
        this->modelMatrix = transMatrix * scaleMatrix;
    }

    void setHeight(float height)
    {
        float scale = modelMatrix[0][0];
        float position_x = modelMatrix[3][0];
        float position_z = modelMatrix[3][2];

        mat4 identity;
        mat4 scaleMatrix = vmath::scale(scale, scale, scale);
        mat4 transMatrix = vmath::translate(position_x, height, position_z);
        this->modelMatrix = transMatrix * scaleMatrix;
    }

    float getHeight()
    {
        return height;
    }

    mat4 getModelMatrix()
    {
        return modelMatrix;
    }

    static const int FBOw = 1920;
    static const int FBOh = 1080;

    FrameBufferObject const &getReflectionFBO()
    {
        return *reflectionFBO;
    }

    void draw()
    {
        // draw water plane
        vmath::mat4 gWorld = modelMatrix;
        vmath::mat4 gVP = perspectiveProjectionMatrix * camera.getViewMatrix();

        glUseProgram(shad->shaderProgramObject);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        this->setHeight(height);

        shad->setMat4("u_modelMatrix", modelMatrix);
        shad->setMat4("u_gVP", gVP);

        shad->setVec3("u_LightColor", vec3(255.0f, 255.0f, 230.0f) / 255.0f);

        vec3 waterLightDirection = vmath::normalize(vec3(0.0f, 3.0f, 0.0f));
        if (USE_FPV_CAM)
            shad->setVec3("u_LightPosition", waterLightDirection * 1e6f + camera.getEye());
        else
            shad->setVec3("u_LightPosition", waterLightDirection * 1e6f + globalBezierCamera->getEye());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, reflectionFBO->tex);
        shad->setInt("reflectionTex", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, refractionFBO->tex);
        shad->setInt("refractionTex", 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, dudvMap);
        shad->setInt("waterDUDV", 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, normalMap);
        shad->setInt("normalMap", 3);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, refractionFBO->depthTex);
        shad->setInt("depthMap", 4);

        float waveSpeed = 0.25;
        float time = ELAPSED_TIME;

        float moveFactor = waveSpeed * time;
        shad->setFloat("moveFactor", moveFactor);

        shad->setVec3("cameraPosition", camera.getEye());

        // waterPlane->Draw(*shad);
        this->drawVertices();
        glDisable(GL_BLEND);
    }

private:
    unsigned int planeVAO, planeVBO, planeEBO;
    float scale, height;
    FrameBufferObject *reflectionFBO;
    FrameBufferObject *refractionFBO;

    unsigned int dudvMap, normalMap;
    mat4 modelMatrix;
    WaterShader *shad;
};
