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
const int INTERNAL_SCENE_1 = 1;
const int INTERNAL_SCENE_2 = 2;

class OutroScene
{

public:
    int CURRENT_INTERNAL_SCENE = INTERNAL_SCENE_1;

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
    GLuint texture_outro;
    GLuint texture_earthNormalMap;

    // shaders
    NoiseSunShader noiseSunShader;
    SmokeEarthShader smokeEarthShader;

    // Fadin Fadeout
    float fadeAlpha = 1.0f;

    // Camera
    BezierCamera sceneCamera1;
    BezierCamera sceneCamera2;
    std::vector<std::vector<float>> bezierPointsScene1 = {
        {-3.599999f, 0.000000f, -1.199999f},
        {-2.599999f, 0.000000f, -0.199999f},
        {-2.000000f, 0.000000f, 0.600001f},
    };

    // YAW GLOBAL
    std::vector<float> yawScene1 = {
        36.000000f,
        36.000000f,
        34.000000f,
    };

    // PITCH GLOBAL
    std::vector<float> pitchScene1 = {
        0.000000f,
        0.000000f,
        0.000000f,
    };

    std::vector<std::vector<float>> bezierPointsScene2 = {
        {-0.500000f, 1.300000f, 1.200001f},
        {-0.700000f, 1.500000f, 1.100001f},
        {-1.300000f, 1.100000f, 1.400001f},
        {-2.300000f, 1.300000f, 1.200001f},
        {-2.300000f, 1.300000f, 1.200001f},
        {-2.300000f, 1.300000f, 1.200001f},
    };

    // YAW GLOBAL
    std::vector<float> yawScene2 = {
        -20.000000f,
        -20.000000f,
        -15.000000f,
        -11.000000f,
        -11.000000f,
        -11.000000f,
    };

    // PITCH Scene2
    std::vector<float> pitchScene2 = {
        -49.000000f,
        -46.000000f,
        -32.000000f,
        -28.000000f,
        -28.000000f,
        -28.000000f,
    };
    float outroScrollY = -4.89f;
    float earthTextureInterpolate = 0.0f;

    // member functions
    OutroScene()
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
        if (LoadPNGImage(&texture_stoneEarth, "./assets/textures/earth/earth_diffuseOriginal.png") == FALSE)
        {
            PrintLog("Failed to load stoneEarth texture\n");
            return FALSE;
        }
        if (LoadPNGImage(&texture_earthNormalMap, "./assets/textures/earth/earth_normal.png") == FALSE)
        {
            PrintLog("Failed to load stoneEarth texture\n");
            return FALSE;
        }
        if (LoadPNGImage(&texture_cloudEarth, "./assets/textures/earth/clouds2.png") == FALSE)
        {
            PrintLog("Failed to load clouds texture\n");
            return FALSE;
        }
        if (LoadPNGImage(&texture_outro, "./assets/textures/outro/outro.png") == FALSE)
        {
            PrintLog("Failed to load clouds texture\n");
            return FALSE;
        }

        isInitialized = true;

        // Camera 1
        sceneCamera1.initialize();
        sceneCamera1.setBezierPoints(bezierPointsScene1, yawScene1, pitchScene1);
        sceneCamera1.update();

        // Camera 2
        sceneCamera2.initialize();
        sceneCamera2.setBezierPoints(bezierPointsScene2, yawScene2, pitchScene2);
        // sceneCamera2.setBezierPoints(bezierPoints, yawGlobal, pitchGlobal);
        sceneCamera2.update();
        return TRUE;
    }

    void display()
    {
        modelMatrix = mat4::identity();
        switch (CURRENT_INTERNAL_SCENE)
        {
        case INTERNAL_SCENE_1:
        {
            setGlobalBezierCamera(&sceneCamera1);
            // sceneCamera1.setBezierPoints(bezierPoints, yawGlobal, pitchGlobal);

            sceneCamera1.update();
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
        }
        break;

        case INTERNAL_SCENE_2:
        {
            setGlobalBezierCamera(&sceneCamera2);
            // sceneCamera2.setBezierPoints(bezierPoints, yawGlobal, pitchGlobal);

            sceneCamera2.update();
            updateGlobalViewMatrix();

            displayScene();
        }
        break;

        default:
            break;
        }

        // FADE IN
        pushMatrix(modelMatrix);
        {
            modelMatrix = vmath::scale(1.0f, 1.0f, 1.0f);
            commonShaders->overlayColorShader->draw(modelMatrix, 0.0f, 0.0f, 0.0f, fadeAlpha);
        }
        modelMatrix = popMatrix();

        // Outro Texture
        pushMatrix(modelMatrix);
        {
            // -4.89 --> 2.0
            modelMatrix = vmath::translate(0.0f, outroScrollY, 0.0f) * vmath::scale(1.0f, 1.0f + 2.799999f, 1.0f);
            commonShaders->overlayTextureShader->drawQuadWithTexture(texture_outro, modelMatrix, 1.0f);
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
        // sceneCamera2.displayBezierCurve();
    }

    void drawBloomObjects()
    {
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix;
            drawEarth(EARTH_STONE);
        }
        modelMatrix = popMatrix();
        // Cloud earth
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix;
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

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture_earthNormalMap);
                glUniform1i(commonShaders->textureLightShader->textureNormalSamplerUniform, 1);
                glUniform1i(commonShaders->textureLightShader->isNormalMapUniform, 1);

                // Second texture
                glUniform1i(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "isMultiTexture"), 1);
                glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "u_time"), ELAPSED_TIME);
                glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "multiTextureInterpolation"), earthTextureInterpolate);

                // Controlling Light position
                if (earthTextureInterpolate >= 1.0f)
                {
                    glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "lightX"), -2.0f + 3.399999f);
                    glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "lightY"), -1.0f - 6.599996f);
                    glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "lightZ"), 1.0f + 12.800013f);
                }
                else
                {
                    glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "lightX"), -2.0f + 2.300000f);
                    glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "lightY"), -1.0f + 0.300000f);
                    glUniform1f(glGetUniformLocation(commonShaders->textureLightShader->shaderProgramObject, "lightZ"), 1.0f + -0.100000f);
                }

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
                if (earthTextureInterpolate >= 1.0)
                    glUniform1f(commonShaders->textureLightShader->alphaValueUniform, 0.2f);
                else
                    glUniform1f(commonShaders->textureLightShader->alphaValueUniform, 0.5f);
                // Controlling Light position
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

    void update()
    {
        ////////////////////////////////// FADING
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
        switch (CURRENT_INTERNAL_SCENE)
        {
        case INTERNAL_SCENE_1:
        {

            // Move camera
            if (sceneCamera1.time <= 1.0f)
                sceneCamera1.time += (0.000015f + 0.00084f);
            // sceneCamera1.time = globalTime;

            // Trigger fadeout
            if (sceneCamera1.time > 0.8f)
            {
                isFadeout = true;
            }

            // Switch Scene and reset values
            if (isFadeout == true && fadeAlpha >= 1.0f)
            {
                isFadeout = false;
                CURRENT_INTERNAL_SCENE = INTERNAL_SCENE_2;
                earthTextureInterpolate = 1.0f;
            }
        }
        break;
        case INTERNAL_SCENE_2:
        {

            // Move camera
            if (sceneCamera2.time <= 1.0f)
                sceneCamera2.time += (0.000015f + 0.00084f);
            // sceneCamera2.time = globalTime;

            // Trigger fadeout
            if (sceneCamera2.time >= 1.0f)
            {
                isFadeout = true;
            }
        }
        break;
        }

        if (outroScrollY < 2.0f)
            outroScrollY += 0.003f;

        // earthTextureInterpolate = 1.0f;
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
