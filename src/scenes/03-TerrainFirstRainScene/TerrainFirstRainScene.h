#pragma once
#include "../../utils/common.h"
#include "../../effects/cubemap/Cubemap.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../shaders/smokeEarth/SmokeEarthShader.h"
#include "../../shaders/textureLight/TextureLightShader.h"
#include "../../shaders/terrainPostProcess/TerrainPostProcess.h"
#include "../../effects/terrain/Terrain.h"
#include "../../effects/water_matrix/WaterMatrix.h"
#include "../../effects/rain/Rain.h"
#include "../../utils/camera/BezierCamera.h"

class TerrainFirstRainScene
{

public:
    bool isInitialized = false;
    Terrain *terrain;
    CubeMap *cubemap;
    WaterMatrix *waterMatrix;
    Rain *rain = NULL;
    BezierCamera sceneCamera;
    // Fadin Fadeout
    float fadeAlpha = 1.0f;
    // member functions
    TerrainFirstRainScene()
    {
        // Terrain
        terrain = new Terrain();
        cubemap = new CubeMap();
        waterMatrix = new WaterMatrix();
        rain = new Rain(20000);
    }

    BOOL initialize()
    {
        // Cubemap
        const char *faces[] =
            {
                "./assets/textures/terrain/cubemap_dark/px.png",
                "./assets/textures/terrain/cubemap_dark/nx.png",
                "./assets/textures/terrain/cubemap_dark/py.png",
                "./assets/textures/terrain/cubemap_dark/ny.png",
                "./assets/textures/terrain/cubemap_dark/pz.png",
                "./assets/textures/terrain/cubemap_dark/nz.png"};
        cubemap = new CubeMap();
        if (!cubemap->initialize(faces))
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

        // Camera
        // Camera
        sceneCamera.initialize();

        isInitialized = true;
        return TRUE;
    }

    void display()
    {
        // Camera
        modelMatrix = mat4::identity();
        viewMatrix = mat4::identity();

        setGlobalBezierCamera(&sceneCamera);
        sceneCamera.setBezierPoints(bezierPoints, yawGlobal, pitchGlobal);
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
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * vmath::scale(500000.0f, 500000.0f, 500000.0f);
            cubemap->display();
        }
        modelMatrix = popMatrix();

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
