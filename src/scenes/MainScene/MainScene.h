#pragma once
#include "../../utils/common.h"
#include "../../utils/camera/Camera.h"
#include "../../utils/camera/BezierCamera.h"

// SCENES
#include "../01-EarthBirthScene/EarthBirthScene.h"
#include "../02-EarthCooldownScene/EarthCooldownScene.h"
#include "../03-TerrainFirstRainScene/TerrainFirstRainScene.h"
#include "../04-TerrainShadowScene/TerrainShadowScene.h"
#include "../05-OutroScene/OutroScene.h"

BOOL bDoneAllInitialization = FALSE;
extern Camera camera;

class MainScene
{

public:
    // member variables
    int selected_scene;
    bool START_E2E_DEMO;

    // Scenes
    EarthBirthScene *earthBirthScene01;
    EarthCooldownScene *earthCooldownScene02;
    TerrainFirstRainScene *terrainFirstRainScene03;
    TerrainShadowScene *terrainShadowScene04;
    OutroScene *outroScene;

    // member functions
    MainScene()
    {
        // SCENE
        earthBirthScene01 = new EarthBirthScene();
        earthCooldownScene02 = new EarthCooldownScene();
        terrainFirstRainScene03 = new TerrainFirstRainScene();
        terrainShadowScene04 = new TerrainShadowScene();
        outroScene = new OutroScene();

        // SCENE CONTROLS
        START_E2E_DEMO = TRUE;
        selected_scene = SCENE_00_BLANK;
    }

    BOOL initialize()
    {
        // SCENES

        if (START_E2E_DEMO)
        {
            terrainShadowScene04->initialize();
            terrainFirstRainScene03->initialize();
            earthCooldownScene02->initialize();
            outroScene->initialize();
            // Need to initialize at the end
            earthBirthScene01->initialize();
        }
        else
        {
            switch (selected_scene)
            {
            case SCENE_01_EARTH_BIRTH:
                earthBirthScene01->initialize();
                break;
            case SCENE_02_EARTH_COOLDOWN:
                earthCooldownScene02->initialize();
                break;
            case SCENE_03_TERRAIN_WITH_HEAVY_RAIN:
                terrainFirstRainScene03->initialize();
                break;
            case SCENE_04_TERRAIN_SHADOW:
                terrainShadowScene04->initialize();
                break;
            case SCENE_05_OUTRO:
                outroScene->initialize();
                break;
            default:
                break;
            }
        }

        return TRUE;
    }

    void display()
    {
        switch (selected_scene)
        {
        case SCENE_01_EARTH_BIRTH:
            earthBirthScene01->display();
            break;
        case SCENE_02_EARTH_COOLDOWN:
            earthCooldownScene02->display();
            break;
        case SCENE_03_TERRAIN_WITH_HEAVY_RAIN:
            terrainFirstRainScene03->display();
            break;
        case SCENE_04_TERRAIN_SHADOW:
            terrainShadowScene04->display();
            break;
        case SCENE_05_OUTRO:
            outroScene->display();
            break;
        default:
            break;
        }
    }

    void update()
    {
        // SCENE SWITCHER
        if (START_E2E_DEMO) // Switch scenes only if end to end demo is played
        {
            if (ELAPSED_TIME >= START_TIME_SCENE_00_BLANK && selected_scene == SCENE_00_BLANK)
            {
                setSelectedScene(SCENE_01_EARTH_BIRTH);
                sdkResetTimer(&timer);
            }
            else if (ELAPSED_TIME > START_TIME_SCENE_02_01_EARTH_COOLDOWN && ELAPSED_TIME < START_TIME_SCENE_03_01_TERRAIN_FIRST_RAIN)
            {
                setSelectedScene(SCENE_02_EARTH_COOLDOWN);
            }
            else if (ELAPSED_TIME > START_TIME_SCENE_03_01_TERRAIN_FIRST_RAIN && ELAPSED_TIME < START_TIME_SCENE_04_TERRAIN_SHADOW)
            {
                setSelectedScene(SCENE_03_TERRAIN_WITH_HEAVY_RAIN);
            }
            else if (ELAPSED_TIME > START_TIME_SCENE_04_TERRAIN_SHADOW && ELAPSED_TIME < (START_TIME_SCENE_04_TERRAIN_SHADOW_END + 1))
            {
                setSelectedScene(SCENE_04_TERRAIN_SHADOW);
            }
            else if (ELAPSED_TIME > (START_TIME_SCENE_04_TERRAIN_SHADOW_END + 1))
            {
                setSelectedScene(SCENE_05_OUTRO);
            }
        }

        switch (selected_scene)
        {
        case SCENE_01_EARTH_BIRTH:
            earthBirthScene01->update();
            break;
        case SCENE_02_EARTH_COOLDOWN:
            earthCooldownScene02->update();
            break;
        case SCENE_03_TERRAIN_WITH_HEAVY_RAIN:
            terrainFirstRainScene03->update();
            break;
        case SCENE_04_TERRAIN_SHADOW:
            terrainShadowScene04->update();
            break;
        case SCENE_05_OUTRO:
            outroScene->update();
            break;
        default:
            break;
        }
    }

    void uninitialize()
    {
        if (earthBirthScene01->isInitialized)
            earthBirthScene01->uninitialize();
        if (earthCooldownScene02->isInitialized)
            earthCooldownScene02->uninitialize();
        if (terrainFirstRainScene03->isInitialized)
            terrainFirstRainScene03->uninitialize();
    }
};
