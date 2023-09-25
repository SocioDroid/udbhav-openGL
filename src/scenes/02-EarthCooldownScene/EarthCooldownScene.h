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

void setGlobalBezierCamera(BezierCamera *bezierCamera);

class EarthCooldownScene
{

public:
    bool isInitialized = false;

    // constants
    static const int EARTH_STONE = 0;
    static const int EARTH_CLOUD = 1;

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

    // Fadin Fadeout
    float fadeAlpha = 1.0f;
    float spinEarthTime = 0.0f;
    // Camera
    BezierCamera sceneCamera;
    std::vector<std::vector<float>> bezierPointsScene = {
        {-6.699996f, 0.000000f, 3.000000f},
        {-6.599996f, 0.000000f, 6.499996f},
        {-4.399998f, 0.000000f, 7.799995f},
        {-3.199999f, -2.000000f, 7.799995f},
        {-0.200000f, -1.900000f, 7.799995f},
        {1.500000f, -1.300000f, 7.799995f},
        {4.499998f, -1.300000f, 7.799995f},
        {4.699998f, -0.600000f, 5.499997f},
        {7.399995f, -0.300000f, 4.299998f},
        {9.700001f, -0.300000f, 2.200000f},
        {11.100006f, -0.300000f, 2.200000f},
        {11.300007f, -0.300000f, 1.700000f},
        {12.000010f, -0.300000f, 1.300000f},
        {12.200010f, -0.200000f, 1.000000f},
        {12.900013f, -0.900000f, -0.400000f},
        {12.900013f, -0.900000f, -0.400000f},
        {13.600016f, -0.900000f, -0.700000f},
        {13.600016f, -0.900000f, -0.700000f},
    };

    // YAW GLOBAL
    std::vector<float> yawScene = {
        -23.000000f,
        -23.000000f,
        -23.000000f,
        -23.000000f,
        -31.000000f,
        -31.000000f,
        -36.000000f,
        -36.000000f,
        -36.000000f,
        -24.000000f,
        -30.000000f,
        -23.000000f,
        -21.000000f,
        -17.000000f,
        0.000000f,
        22.000000f,
        22.000000f,
        22.000000f,
    };

    // PITCH GLOBAL
    std::vector<float> pitchScene = {
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        1.000000f,
        2.000000f,
        2.000000f,
        -1.000000f,
        0.000000f,
        1.000000f,
        1.000000f,
        1.000000f,
        1.000000f,
        2.000000f,
        15.000000f,
        21.000000f,
        21.000000f,
        21.000000f,
    };

    float earthTextureInterpolate = 0.0f;
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
        bloomShader.exposure = 0.9f;
        bloomShader.blurAmount = 10;
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
        isInitialized = true;

        // Camera
        sceneCamera.initialize();
        sceneCamera.setBezierPoints(bezierPointsScene, yawScene, pitchScene);
        sceneCamera.update();
        return TRUE;
    }

    void display()
    {
        setGlobalBezierCamera(&sceneCamera);
        sceneCamera.update();
        updateGlobalViewMatrix();

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

        // FADE IN
        pushMatrix(modelMatrix);
        {
            modelMatrix = vmath::scale(1.0f, 1.0f, 1.0f);
            commonShaders->overlayColorShader->draw(modelMatrix, 0.0f, 0.0f, 0.0f, fadeAlpha);
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
            modelMatrix = modelMatrix * vmath::translate(0.0f, 0.0f, 100.102524f);
            starfield->display();
        }
        modelMatrix = popMatrix();

        // Camera
        // sceneCamera.displayBezierCurve();
    }
    float lerp(float a, float b, float time)
    {
        return a + (b - a) * time;
    }
    void drawBloomObjects()
    {
        if (sceneCamera.time < 0.35f)
        {
            pushMatrix(modelMatrix);
            {
                drawSun();
            }
            modelMatrix = popMatrix();
        }
        pushMatrix(modelMatrix);
        {

            pushMatrix(modelMatrix);
            {
                modelMatrix = modelMatrix * translate(15.0f, 0.0f, 0.0f) * vmath::rotate(lerp(100000.0f, 0.0f, spinEarthTime) - (ELAPSED_TIME * 4.0f), 0.0f, 1.0f, 0.0f);
                drawEarth(EARTH_STONE);
            }
            modelMatrix = popMatrix();
            // Cloud earth
            pushMatrix(modelMatrix);
            {
                modelMatrix = modelMatrix * translate(15.0f, 0.0f, 0.0f) * vmath::rotate(lerp(100000.0f, 0.0f, spinEarthTime), 0.0f, 1.0f, 0.0f);
                drawEarth(EARTH_CLOUD);
            }
            modelMatrix = popMatrix();
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
            glUseProgram(commonShaders->textureLightShader->shaderProgramObject);
            pushMatrix(modelMatrix);
            {
                modelMatrix = modelMatrix * vmath::rotate(-90.0f, 1.0f, 0.0f, 0.0f);
                modelMatrix = modelMatrix * vmath::rotate(-90.0f - (ELAPSED_TIME * 0.2f), 0.0f, 0.0f, 1.0f);
                /* Initialize uniforms constant throughout rendering loop. */
                glUniformMatrix4fv(commonShaders->textureLightShader->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
                glUniformMatrix4fv(commonShaders->textureLightShader->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
                glUniformMatrix4fv(commonShaders->textureLightShader->projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

                glEnable(GL_TEXTURE_2D);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_stoneEarth);
                glUniform1i(commonShaders->textureLightShader->textureSamplerUniform, 0);

                // Second texture
                glUniform1i(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "isLight"), 0);
                glUniform1i(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "isMultiTexture"), 1);
                glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "u_time"), ELAPSED_TIME);
                glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "multiTextureInterpolation"), earthTextureInterpolate);

                // Controlling Light position
                glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "lightX"), -2.0f + 2.300000f);
                glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "lightY"), -1.0f + 0.300000f);
                glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "lightZ"), 1.0f + -0.100000f);

                glBindVertexArray(earthSphere->vao_sphere);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, earthSphere->getNumberOfSphereVertices());
                glBindVertexArray(0);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            modelMatrix = popMatrix();
            glUseProgram(0);
            break;
        case EARTH_CLOUD:

            glUseProgram(commonShaders->textureLightShader->shaderProgramObject);
            pushMatrix(modelMatrix);
            {
                modelMatrix = modelMatrix * vmath::rotate(-90.0f, 1.0f, 0.0f, 0.0f);
                modelMatrix = modelMatrix * vmath::rotate(-190.0f + ELAPSED_TIME, 0.0f, 0.0f, 1.0f);
                modelMatrix = modelMatrix * vmath::scale(1.02f, 1.02f, 1.02f);
                /* Initialize uniforms constant throughout rendering loop. */
                glUniformMatrix4fv(commonShaders->textureLightShader->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
                glUniformMatrix4fv(commonShaders->textureLightShader->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
                glUniformMatrix4fv(commonShaders->textureLightShader->projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glEnable(GL_TEXTURE_2D);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture_cloudEarth);
                glUniform1i(commonShaders->textureLightShader->textureSamplerUniform, 0);
                glUniform1f(commonShaders->textureLightShader->alphaValueUniform, 0.3f);

                // Controlling Light position
                glUniform1i(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "isLight"), 0);
                glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "lightX"), -2.0f + 2.300000f);
                glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "lightY"), -1.0f + 0.300000f);
                glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "lightZ"), 1.0f + -0.100000f);

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
    bool isFadeout = false;
    float spinEarthTimeIncr = 0.0003f;
    void update()
    {
        ////////////////////////////////// FADI NG
        // Fade In
        if (!isFadeout)
        {
            if (fadeAlpha > 0.0f)
                fadeAlpha -= 0.01f;
        }
        else
        {
            // ////// FADEOUT SPEED
            if (fadeAlpha <= 1.0f)
                fadeAlpha += 0.008f;
        }
        // Trigger fadeout
        if (ELAPSED_TIME > (START_TIME_SCENE_03_01_TERRAIN_FIRST_RAIN - 3))
        {
            isFadeout = true;
        }

        if (sceneCamera.time <= 1.0f)
            sceneCamera.time += (0.000015f + 0.00034f);

        if (sceneCamera.time > 0.2f)
        {
            if (earthTextureInterpolate < 0.97f)
            {
                earthTextureInterpolate += 0.0007f;
            }
        }

        if (spinEarthTime < 1.0f)
        {
            if (spinEarthTime > 0.2)
            {
                if (spinEarthTimeIncr > 0.0f)
                    spinEarthTimeIncr -= 0.00000015f;
            }
            spinEarthTime += spinEarthTimeIncr;
        }
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
