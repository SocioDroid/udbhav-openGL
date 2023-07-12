#pragma once
#include "../../utils/common.h"
#include "../../includes/SphereAish.h"
#include "../../effects/cubemap/Cubemap.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../effects/starfield/starfield.h"
#include "../../shaders/bloom/HDR_Shader.h"
#include "../../shaders/noiseSun/NoiseSunShader.h"
#include "../../effects/bloom/Bloom_Shaders.h"

void setGlobalBezierCamera(BezierCamera *bezierCamera);

class EarthCooldownScene
{

public:
    // member variables
    int selected_scene;
    bool START_E2E_DEMO;

    SphereAish *sunSphere;
    SphereAish *earthSphere;
    StarField *starfield;
    CubeMap *cubemap;

    // Bloom
    BloomShaders bloomShader;
    GLuint fbo_UserMap; // FRAME BUFFER OBJECT
    GLuint rbo_UserMap; // RENDER BUFFER OBJECT
    GLuint fbo_texture_UserMap;
    GLuint texture_stoneEarth;

    // shaders
    NoiseSunShader noiseSunShader;

    // member functions
    EarthCooldownScene()
    {
        starfield = new StarField();
        cubemap = new CubeMap();
    }

    BOOL initialize()
    {
        // Bloom
        bloomShader.initialize_bloomShaderObject();
        bloomShader.exposure = 0.2f;
        if (bloomShader.CreateSceneFBO(1920, 1080, fbo_UserMap, rbo_UserMap, fbo_texture_UserMap) == false)
        {
            PrintLog("Failed to create Scene FBO\n");
            return FALSE;
        }

        // Starfield
        starfield->initialize();

        // Cubemap
        const char *faces[] =
            {
                "./assets/textures/starfield/cubemap/px.png",
                "./assets/textures/starfield/cubemap/nx.png",
                "./assets/textures/starfield/cubemap/py.png",
                "./assets/textures/starfield/cubemap/ny.png",
                "./assets/textures/starfield/cubemap/pz.png",
                "./assets/textures/starfield/cubemap/nz.png"};
        cubemap = new CubeMap();
        if (!cubemap->initialize(faces))
        {
            PrintLog("Failed to initialize starfield CubeMap");
            return FALSE;
        }

        // Spheres
        noiseSunShader.initialize();
        sunSphere = new SphereAish(1.0f, 100, 100);
        earthSphere = new SphereAish(0.3f, 100, 100);
        if (LoadPNGImage(&texture_stoneEarth, "./assets/textures/earth/cooledEarth3.png") == FALSE)
        {
            PrintLog("Failed to load stoneEarth texture\n");
            return FALSE;
        }
        return TRUE;
    }

    void display()
    {
        pushMatrix(modelMatrix);
        {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo_UserMap);
            {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glViewport(0, 0, 1920, 1080);
                displayScene();
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        modelMatrix = popMatrix();

        pushMatrix(modelMatrix);
        {

            bloomShader.bindBloomFBO();
            {
                // Applying bloom only to spheres
                drawBloomObjects();
            }
            bloomShader.unbindBloomFBO();

            bloomShader.renderBlurFBO();
            bloomShader.renderFinalBloomScene(fbo_texture_UserMap);
        }
        modelMatrix = popMatrix();
    }

    void displayScene()
    {
        // Cubemap
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * vmath::scale(1000.0f, 1000.0f, 1000.0f);
            cubemap->display();
        }
        modelMatrix = popMatrix();

        pushMatrix(modelMatrix);
        {
            drawBloomObjects();
        }
        modelMatrix = popMatrix();

        pushMatrix(modelMatrix);
        {
            starfield->display();
        }
        modelMatrix = popMatrix();
    }

    void drawBloomObjects()
    {
        pushMatrix(modelMatrix);
        {
            drawSun();
        }
        modelMatrix = popMatrix();
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * translate(3.0f, 0.0f, 0.0f);
            drawEarth();
        }
        modelMatrix = popMatrix();
    }
    void drawSun()
    {
        glUseProgram(noiseSunShader.shaderProgramObject);
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * vmath::rotate(-90.0f, 1.0f, 0.0f, 0.0f);
            /* Initialize uniforms constant throughout rendering loop. */
            glUniformMatrix4fv(noiseSunShader.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
            glUniformMatrix4fv(noiseSunShader.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
            glUniformMatrix4fv(noiseSunShader.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
            glUniform1f(noiseSunShader.timeUniform, ELAPSED_TIME);
            glBindVertexArray(sunSphere->vao_sphere);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, sunSphere->getNumberOfSphereVertices());
            glBindVertexArray(0);
        }
        modelMatrix = popMatrix();
        glUseProgram(0);
    }
    void drawEarth()
    {
        glUseProgram(commonShaders->textureShader->shaderProgramObject);
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * vmath::rotate(-90.0f, 1.0f, 0.0f, 0.0f);
            /* Initialize uniforms constant throughout rendering loop. */
            glUniformMatrix4fv(commonShaders->textureShader->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
            glUniformMatrix4fv(commonShaders->textureShader->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
            glUniformMatrix4fv(commonShaders->textureShader->projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

            glEnable(GL_TEXTURE_2D);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_stoneEarth);
            glUniform1i(commonShaders->textureShader->textureSamplerUniform, 0);

            glBindVertexArray(earthSphere->vao_sphere);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, earthSphere->getNumberOfSphereVertices());
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        modelMatrix = popMatrix();
        glUseProgram(0);
    }
    void update()
    {
    }
    void uninitialize()
    {
        if (starfield)
        {
            starfield->uninitialize();
            delete (starfield);
            starfield = NULL;
        }
        if (cubemap)
        {
            cubemap->uninitialize();
            delete (cubemap);
            cubemap = NULL;
        }
    }
};
