#pragma once
#include "../../utils/common.h"
#include "../../effects/cubemapMerge/CubemapMerge.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../shaders/smokeEarth/SmokeEarthShader.h"
#include "../../includes/SphereAish.h"
#include "../../shaders/terrainPostProcess/TerrainPostProcess.h"
#include "../../shaders/noiseCloud/NoiseCloud.h"
#include "../../effects/terrain/Terrain.h"
#include "../../effects/water_matrix/WaterMatrix.h"
#include "../../effects/rain/Rain.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../utils/noise/Noise.h"

class TerrainFirstRainScene
{

public:
    bool isInitialized = false;
    Terrain *terrain;
    CubeMapMerge *cubemap;
    WaterMatrix *waterMatrix;
    Rain *rain = NULL;
    BezierCamera sceneCamera;
    SphereAish *sphereCloud;
    NoiseCloudShader noiseCloudShader;
    GLuint texture_noise;

    // Fadein Fadeout
    float fadeAlpha = 1.0f;
    float cubemapFactor = 0.0f;
    float scaleFactor = 2.0f;

    // Cloud
    float cloudAlpha = 0.0f;

    // Camera

    std::vector<std::vector<float>> bezierPointsScene = {
        {-1400.000000f, 2018.000000f, 14210.000000f},
        {-1050.000000f, 2018.000000f, 11550.000000f},
        {-630.000000f, 2018.000000f, 5600.000000f},
        {-2590.000000f, 2018.000000f, 2870.000000f},
        {-3010.000000f, 2578.000000f, -210.000000f},
        {-2730.000000f, 3488.000000f, -4060.000000f},
        {-2940.000000f, 4468.000000f, -5880.000000f},
        {-1680.000000f, 4538.000000f, -7560.000000f},
        {-350.000000f, 4538.000000f, -10570.000000f},
        {1190.000000f, 4958.000000f, -11340.000000f},
        {2590.000000f, 5448.000000f, -14910.000000f},
        {350.000000f, 5448.000000f, -15890.000000f},
        {-1540.000000f, 5448.000000f, -17360.000000f},
        {-5950.000000f, 5448.000000f, -25270.000000f},
        {-10290.000000f, 5448.000000f, -30030.000000f},
        {-6930.000000f, 5448.000000f, -33810.000000f},
        {7280.000000f, 5588.000000f, -25270.000000f},
        {9170.000000f, 5588.000000f, -25270.000000f},
        {11410.000000f, 5588.000000f, -26180.000000f},
        {12460.000000f, 5448.000000f, -26180.000000f},
        {18410.000000f, 5098.000000f, -26390.000000f},
        {20160.000000f, 5098.000000f, -26390.000000f},
        {24500.000000f, 4958.000000f, -26460.000000f},
        {28070.000000f, 4328.000000f, -26460.000000f},
        {30030.000000f, 2648.000000f, -26460.000000f},
        {35700.000000f, 2158.000000f, -26460.000000f},
        {35420.000000f, 2158.000000f, -24500.000000f},
        {35420.000000f, 2158.000000f, -19390.000000f},
        {35630.000000f, 2158.000000f, -18340.000000f},
        {35840.000000f, 1458.000000f, -15610.000000f},
        {34720.000000f, 1318.000000f, -14700.000000f},
        {30730.000000f, 1318.000000f, -13440.000000f},
        {28210.000000f, 1318.000000f, -13370.000000f},
        {27300.000000f, 1318.000000f, -13370.000000f},
        {26040.000000f, 1038.000000f, -13370.000000f},
        {24290.000000f, 1038.000000f, -13370.000000f},
        {21070.000000f, 828.000000f, -13370.000000f},
        {20930.000000f, 828.000000f, -13370.000000f},
        {20860.000000f, 828.000000f, -13370.000000f},
        {20790.000000f, 828.000000f, -13370.000000f},
        {20790.000000f, 828.000000f, -13370.000000f},
    };

    // YAW GLOBAL
    std::vector<float> yawScene = {
        -90.000000f,
        -90.000000f,
        -90.000000f,
        -82.000000f,
        -82.000000f,
        -72.000000f,
        -64.000000f,
        -60.000000f,
        -58.000000f,
        -54.000000f,
        -52.000000f,
        -42.000000f,
        -34.000000f,
        0.000000f,
        24.000000f,
        54.000000f,
        76.000000f,
        94.000000f,
        94.000000f,
        94.000000f,
        94.000000f,
        106.000000f,
        106.000000f,
        106.000000f,
        106.000000f,
        122.000000f,
        126.000000f,
        128.000000f,
        158.000000f,
        170.000000f,
        170.000000f,
        174.000000f,
        174.000000f,
        170.000000f,
        160.000000f,
        140.000000f,
        130.000000f,
        130.000000f,
        130.000000f,
        130.000000f,
        130.000000f,
    };

    // PITCH Scene
    std::vector<float> pitchScene = {
        -4.000000f,
        -4.000000f,
        -2.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        -4.000000f,
        -4.000000f,
        52.000000f,
        -4.000000f,
        -4.000000f,
        -12.000000f,
        -12.000000f,
        -14.000000f,
        -14.000000f,
        -14.000000f,
        -14.000000f,
        -12.000000f,
        -12.000000f,
        -12.000000f,
        -12.000000f,
        -6.000000f,
        -4.000000f,
        -6.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        4.000000f,
        4.000000f,
        4.000000f,
        4.000000f,
        4.000000f,
    };

    // member functions
    TerrainFirstRainScene()
    {
        // Terrain
        terrain = new Terrain();
        cubemap = new CubeMapMerge();
        waterMatrix = new WaterMatrix(1000. * 80.);
        rain = new Rain(30000);
    }

    BOOL initialize()
    {
        // Cubemap
        const char *facesDark[] =
            {
                "./assets/textures/terrain/cubemap_dark/px.png",
                "./assets/textures/terrain/cubemap_dark/nx.png",
                "./assets/textures/terrain/cubemap_dark/py.png",
                "./assets/textures/terrain/cubemap_dark/ny.png",
                "./assets/textures/terrain/cubemap_dark/pz.png",
                "./assets/textures/terrain/cubemap_dark/nz.png"};

        // Cubemap
        const char *facesLight[] =
            {
                "./assets/textures/terrain/cubemap_light/px.png",
                "./assets/textures/terrain/cubemap_light/nx.png",
                "./assets/textures/terrain/cubemap_light/py.png",
                "./assets/textures/terrain/cubemap_light/ny.png",
                "./assets/textures/terrain/cubemap_light/pz.png",
                "./assets/textures/terrain/cubemap_light/nz.png"};

        cubemap = new CubeMapMerge();

        if (!cubemap->initialize(facesDark, facesLight))
        {
            PrintLog("Failed to initialize starfield CubeMap");
            return FALSE;
        }

        // Water
        waterMatrix->initialize();

        // Heavy Rain
        if (!rain->initialize(2))
        {
            PrintLog("\nFailed to initialize rain\n");
            return FALSE;
        }

        // // Cloud
        sphereCloud = new SphereAish(1.0f, 100, 100);
        if (!noiseCloudShader.initialize())
        {
            PrintLog("\nFailed to initialize NoisCloud Shader\n");
            return FALSE;
        }
        CreateNoise3D(&texture_noise);

        // Camera
        sceneCamera.initialize();
        sceneCamera.setBezierPoints(bezierPointsScene, yawScene, pitchScene);
        sceneCamera.isWater = true;
        isInitialized = true;
        return TRUE;
    }

    void display()
    {
        // Camera
        modelMatrix = mat4::identity();
        viewMatrix = mat4::identity();

        setGlobalBezierCamera(&sceneCamera);
        // sceneCamera.setBezierPoints(bezierPoints, yawGlobal, pitchGlobal);
        sceneCamera.update();
        updateGlobalViewMatrix();

        // Water FBO
        {
            // Reflection
            waterMatrix->bindReflectionFBO(1920, 1080);
            {
                displayScene(1.0);
            }
            waterMatrix->unbindReflectionFBO();

            // Refraction
            waterMatrix->bindRefractionFBO(1920, 1080);
            {
                displayScene(-1.0);
            }
            waterMatrix->unbindRefractionFBO();
        }
        // Actual Scene
        displayScene(1.0);

        // Water Bed
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * translate(0.0f, -56.900028f, 0.0f);
            waterMatrix->renderWaterQuad(terrain->getWaterHeight());
        }
        modelMatrix = popMatrix();

        if (rain->alpha > 0.0f)
            drawRain();

        // FADE IN
        pushMatrix(modelMatrix);
        {
            modelMatrix = vmath::scale(1.0f, 1.0f, 1.0f);
            commonShaders->overlayColorShader->draw(modelMatrix, 0.0f, 0.0f, 0.0f, fadeAlpha);
        }
        modelMatrix = popMatrix();
        // sceneCamera.displayBezierCurve();
    }

    void displayScene(float terrainUp)
    {
        // cubemap
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * vmath::scale(500000.0f, 500000.0f, 500000.0f);
            cubemap->display(cubemapFactor);
        }
        modelMatrix = popMatrix();

        // Cloud
        if (cloudAlpha < 1.0f)
        {
            pushMatrix(modelMatrix);
            {
                modelMatrix = modelMatrix * scale(100000.0f, 100000.0f, 100000.0f) * vmath::rotate(-90.0f, 1.0f, 0.0f, 0.0f);
                drawCloudNoise();
            }
            modelMatrix = popMatrix();
        }

        // Terrain
        pushMatrix(modelMatrix);
        {
            terrain->up = terrainUp;
            terrain->draw();
        }
        modelMatrix = popMatrix();
    }
    void drawRain()
    {
        // RAIN
        pushMatrix(modelMatrix);
        {
            rain->lightAmbient[0] = 0.0f;
            rain->lightAmbient[1] = 0.0f;
            rain->lightAmbient[2] = 0.0f;
            rain->lightAmbient[3] = 1.0f;

            rain->lightDiffuse[0] = 1.0f;
            rain->lightDiffuse[1] = 1.0f;
            rain->lightDiffuse[2] = 1.0f;
            rain->lightDiffuse[3] = 1.0f;

            rain->lightPosition[0] = 0.0f;
            rain->lightPosition[1] = 100.0f;
            rain->lightPosition[2] = -30.0f;
            rain->lightPosition[3] = 1.0f;

            rain->lightSpecular[0] = 1.0f;
            rain->lightSpecular[1] = 1.0f;
            rain->lightSpecular[2] = 1.0f;
            rain->lightSpecular[3] = 1.0f;
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            rain->display();
            glDisable(GL_BLEND);
        }
        modelMatrix = popMatrix();
    }
    /// Helper functions
    float randFloat(float min, float max)
    {
        return min + (rand() / (RAND_MAX / (max - min)));
    }
    void drawCloudNoise()
    {
        glUseProgram(noiseCloudShader.shaderProgramObject);
        pushMatrix(modelMatrix);
        {
            /* Initialize uniforms constant throughout rendering loop. */
            glUniformMatrix4fv(noiseCloudShader.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
            glUniformMatrix4fv(noiseCloudShader.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
            glUniformMatrix4fv(noiseCloudShader.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
            glUniform3fv(noiseCloudShader.lightPosUniform, 1, vec3(0.0f, 5.0f, 1.0f));

            // Lightning
            if (fmod(ELAPSED_TIME, 15) >= 0 && fmod(ELAPSED_TIME, 15) <= randFloat(0.0f, 0.3f) && sceneCamera.time < 0.6)
                glUniform3fv(noiseCloudShader.Color1Uniform, 1, vec3(1.0f, 1.0f, 1.0f));
            else
            {
                if (fmod(ELAPSED_TIME, 3) >= 0 && fmod(ELAPSED_TIME, 3) <= randFloat(0.0f, 0.1f) && sceneCamera.time < 0.6)
                    glUniform3fv(noiseCloudShader.Color1Uniform, 1, vec3(1.0f, 1.0f, 1.0f));
                else
                    glUniform3fv(noiseCloudShader.Color1Uniform, 1, vec3(0.2, 0.2, 0.2));
            }

            glUniform3fv(noiseCloudShader.Color2Uniform, 1, vec3(0.0, 0.0, 0.0));
            glUniform1f(noiseCloudShader.scaleUniform, scaleFactor * 0.1f);
            glUniform1f(noiseCloudShader.alpha, cloudAlpha);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_3D, texture_noise);
            glUniform1i(noiseCloudShader.textureSamplerUniform, 0);

            glBindVertexArray(sphereCloud->vao_sphere);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, sphereCloud->getNumberOfSphereVertices());
            glBindVertexArray(0);
            glDisable(GL_BLEND);
        }
        modelMatrix = popMatrix();
        glUseProgram(0);

        scaleFactor = scaleFactor + 0.001f;
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

        // Camera
        if (sceneCamera.time < 1.0f)
            sceneCamera.time += (0.000015f + 0.00037f);

        // sceneCamera.time = globalTime;

        terrain->updateTilesPositions();
        // Transition to green texture
        if (terrain->getTextureTransitionFactor() < 1.0f)
        {
            terrain->setTextureTransitionFactor(terrain->getTextureTransitionFactor() + 0.0004f);
        }
        // Grass Coverage
        if (terrain->getGrassCoverage() < 0.56f)
        {
            terrain->setGrassCoverage(terrain->getGrassCoverage() + 0.0003f);
        }
        // // Water Height
        if (sceneCamera.time > 0.6f)
        {
            if (terrain->getWaterHeight() < 589.0f)
            {
                terrain->setWaterHeight(terrain->getWaterHeight() + 1.0f);
            }
            // Water Color
            if (waterMatrix->interpolateWaterColor < 1.13f)
            {
                waterMatrix->interpolateWaterColor += 0.001f;
            }
        }
        // Cubemap Transition
        if (sceneCamera.time > 0.5f && cubemapFactor < 1.0f)
        {
            cubemapFactor += 0.001f;
        }
        // Rain alpha
        if (sceneCamera.time > 0.5f && rain->alpha > 0.0f)
        {
            rain->alpha -= 0.002f;
        }
        // Trigger fadeout
        if (ELAPSED_TIME > (START_TIME_SCENE_04_TERRAIN_SHADOW - 3))
        {
            isFadeout = true;
        }
        // Cloud Alpha
        if (sceneCamera.time > 0.6)
        {
            if (cloudAlpha < 1.0f)
            {
                cloudAlpha += 0.001f;
            }
        }
        // terrain->setWaterHeight(scaleX);
        // terrain->setTextureTransitionFactor(1.0f);
    }

    void uninitialize()
    {
        if (terrain)
        {
        }
        if (cubemap)
        {
            cubemap->uninitialize();
            delete (cubemap);
            cubemap = NULL;
        }
    }
};
