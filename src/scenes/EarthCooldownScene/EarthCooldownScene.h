#pragma once
#include "../../utils/common.h"
#include "../../includes/SphereAish.h"
#include "../../effects/cubemap/Cubemap.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../effects/starfield/starfield.h"
#include "../../shaders/bloom/HDR_Shader.h"
#include "../../shaders/noiseSun/NoiseSunShader.h"
#include "../../shaders/smokeEarth/SmokeEarthShader.h"
#include "../../effects/bloom/Bloom_Shaders.h"
#include "../../shaders/textureLight/TextureLightShader.h"

void setGlobalBezierCamera(BezierCamera *bezierCamera);

class EarthCooldownScene
{

public:
    // constants
    static const int EARTH_STONE = 0;
    static const int EARTH_CLOUD = 1;
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
    GLuint texture_cloudEarth;

    // shaders
    NoiseSunShader noiseSunShader;
    SmokeEarthShader smokeEarthShader;
    TextureLightShader textureLightShader;

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
        smokeEarthShader.initialize();
        textureLightShader.initialize();
        sunSphere = new SphereAish(5.0f, 100, 100);
        earthSphere = new SphereAish(1.0f, 100, 100);
        if (LoadPNGImage(&texture_stoneEarth, "./assets/textures/earth/cooledEarth3.png") == FALSE)
        {
            PrintLog("Failed to load stoneEarth texture\n");
            return FALSE;
        }
        if (LoadPNGImage(&texture_cloudEarth, "./assets/textures/earth/clouds2.png") == FALSE)
        {
            PrintLog("Failed to load clouds texture\n");
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
            modelMatrix = modelMatrix * translate(15.0f, 0.0f, 0.0f);
            drawEarth(EARTH_STONE);
        }
        modelMatrix = popMatrix();
        // Cloud earth
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * translate(15.0f, 0.0f, 0.0f);
            drawEarth(EARTH_CLOUD);
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
    void drawEarth(const int EARTH_TYPE)
    {
        switch (EARTH_TYPE)
        {
        case EARTH_STONE:
            glUseProgram(textureLightShader.shaderProgramObject);
            pushMatrix(modelMatrix);
            {
                modelMatrix = modelMatrix * vmath::rotate(-90.0f, 1.0f, 0.0f, 0.0f);
                modelMatrix = modelMatrix * vmath::rotate(-90.0f - (ELAPSED_TIME * 0.2f), 0.0f, 0.0f, 1.0f);
                /* Initialize uniforms constant throughout rendering loop. */
                glUniformMatrix4fv(textureLightShader.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
                glUniformMatrix4fv(textureLightShader.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
                glUniformMatrix4fv(textureLightShader.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

                glEnable(GL_TEXTURE_2D);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_stoneEarth);
                glUniform1i(textureLightShader.textureSamplerUniform, 0);

                // Controlling Light position
                glUniform1f(glGetUniformLocation(textureLightShader.shaderProgramObject, "lightX"), -2.0f + 2.300000f);
                glUniform1f(glGetUniformLocation(textureLightShader.shaderProgramObject, "lightY"), -1.0f + 0.300000f);
                glUniform1f(glGetUniformLocation(textureLightShader.shaderProgramObject, "lightZ"), 1.0f + -0.100000f);

                glBindVertexArray(earthSphere->vao_sphere);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, earthSphere->getNumberOfSphereVertices());
                glBindVertexArray(0);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            modelMatrix = popMatrix();
            glUseProgram(0);
            break;
        case EARTH_CLOUD:

            glUseProgram(textureLightShader.shaderProgramObject);
            pushMatrix(modelMatrix);
            {
                modelMatrix = modelMatrix * vmath::rotate(-90.0f, 1.0f, 0.0f, 0.0f);
                modelMatrix = modelMatrix * vmath::rotate(-90.0f + ELAPSED_TIME, 0.0f, 0.0f, 1.0f);
                modelMatrix = modelMatrix * vmath::scale(1.02f, 1.02f, 1.02f);
                /* Initialize uniforms constant throughout rendering loop. */
                glUniformMatrix4fv(textureLightShader.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
                glUniformMatrix4fv(textureLightShader.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
                glUniformMatrix4fv(textureLightShader.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glEnable(GL_TEXTURE_2D);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_cloudEarth);
                glUniform1i(textureLightShader.textureSamplerUniform, 0);
                glUniform1f(textureLightShader.alphaValueUniform, 0.5f);

                // Controlling Light position
                glUniform1f(glGetUniformLocation(textureLightShader.shaderProgramObject, "lightX"), -2.0f + 2.300000f);
                glUniform1f(glGetUniformLocation(textureLightShader.shaderProgramObject, "lightY"), -1.0f + 0.300000f);
                glUniform1f(glGetUniformLocation(textureLightShader.shaderProgramObject, "lightZ"), 1.0f + -0.100000f);

                glBindVertexArray(earthSphere->vao_sphere);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, earthSphere->getNumberOfSphereVertices());
                glBindVertexArray(0);
                glBindTexture(GL_TEXTURE_2D, 0);

                glDisable(GL_BLEND);
            }
            modelMatrix = popMatrix();
            glUseProgram(0);
            break;
        }
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
