#pragma once
#include "../../utils/common.h"
#include "../../effects/cubemap/Cubemap.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../shaders/smokeEarth/SmokeEarthShader.h"
#include "../../shaders/textureLight/TextureLightShader.h"
#include "../../shaders/terrainPostProcess/TerrainPostProcess.h"
#include "../../effects/terrain/Terrain.h"
#include "../../effects/water_matrix/WaterMatrix.h"

class TerrainFirstRainScene
{

public:
    bool isInitialized = false;
    Terrain *terrain;
    CubeMap *cubemap;
    WaterMatrix *waterMatrix;

    // member functions
    TerrainFirstRainScene()
    {
        // Terrain
        terrain = new Terrain();
        cubemap = new CubeMap();
        waterMatrix = new WaterMatrix();
    }

    BOOL initialize()
    {
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

        waterMatrix->initialize();

        isInitialized = true;
        return TRUE;
    }

    void display()
    {
        terrain->updateTilesPositions();

        // Reflection
        waterMatrix->bindReflectionFBO(1920, 1080);
        pushMatrix(modelMatrix);
        {
            terrain->up = 1.0;
            terrain->draw();
        }
        modelMatrix = popMatrix();
        waterMatrix->unbindReflectionFBO();

        // Refraction
        waterMatrix->bindRefractionFBO(1920, 1080);
        pushMatrix(modelMatrix);
        {
            terrain->up = -1.0;
            terrain->draw();
        }
        modelMatrix = popMatrix();
        waterMatrix->unbindRefractionFBO();
        
        pushMatrix(modelMatrix);
        {
            terrain->draw();
        }
        modelMatrix = popMatrix();
        
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * translate(0.0f, -26.900028f, 0.0f);
            waterMatrix->renderWaterQuad();
        }
        modelMatrix = popMatrix();
    }

    void update()
    {
        // Transition to green texture
        if (terrain->getTextureTransitionFactor() < 1.0f)
        {
            terrain->setTextureTransitionFactor(terrain->getTextureTransitionFactor() + 0.001f);
        }
        // Grass Coverage
        if (terrain->getGrassCoverage() < 0.56f)
        {
            terrain->setGrassCoverage(terrain->getGrassCoverage() + 0.0002f);
        }
        terrain->setTextureTransitionFactor(1.0f);
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
