#pragma once
#include "../../utils/common.h"
#include "../../effects/cubemap/Cubemap.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../includes/SphereAish.h"
#include "../../shaders/terrainPostProcess/TerrainPostProcess.h"
#include "../../shaders/noiseCloud/NoiseCloud.h"
#include "../../effects/terrain/Terrain.h"
#include "../../effects/water_matrix/WaterMatrix.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../utils/noise/Noise.h"

class TerrainShadowScene
{

public:
    bool isInitialized = false;
    Terrain *terrain;
    CubeMap *cubemap;
    WaterMatrix *waterMatrix;
    BezierCamera sceneCamera;
    SphereAish *sphereCloud;

    // Fadein Fadeout
    float fadeAlpha = 1.0f;
    float scaleFactor = 2.0f;

    // member functions
    TerrainShadowScene()
    {
        // Terrain
        terrain = new Terrain();
        cubemap = new CubeMap();
        waterMatrix = new WaterMatrix();
    }

    BOOL initialize()
    {

        // Cubemap
        const char *facesLight[] =
            {
                "./assets/textures/terrain/cubemap_light/px.png",
                "./assets/textures/terrain/cubemap_light/nx.png",
                "./assets/textures/terrain/cubemap_light/py.png",
                "./assets/textures/terrain/cubemap_light/ny.png",
                "./assets/textures/terrain/cubemap_light/pz.png",
                "./assets/textures/terrain/cubemap_light/nz.png"};

        if (!cubemap->initialize(facesLight))
        {
            PrintLog("Failed to initialize CubeMap");
            return FALSE;
        }

        // Water
        waterMatrix->initialize();

        // Camera
        sceneCamera.initialize();
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
            cubemap->display();
        }
        modelMatrix = popMatrix();

        // Terrain
        pushMatrix(modelMatrix);
        {
            terrain->up = terrainUp;
            terrain->draw();
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

        // sceneCamera.time = globalTime;
        terrain->updateTilesPositions();

        // // Trigger fadeout
        // if (ELAPSED_TIME > (START_TIME_SCENE_04_TERRAIN_SHADOW - 3))
        // {
        //     isFadeout = true;
        // }
        terrain->setGrassCoverage(0.66f);
        terrain->setTextureTransitionFactor(1.0f);
        terrain->setWaterHeight(589.0f);
        waterMatrix->interpolateWaterColor = 1.13f;
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
