#pragma once
#include "../../utils/common.h"
#include "../../utils/camera/Camera.h"  
#include "../../utils/camera/BezierCamera.h"

// SCENES
#include "../EarthBirthScene/EarthBirthScene.h"
#include "../EarthCooldownScene/EarthCooldownScene.h"

#include "../../effects/terrain/Terrain.h"

void setGlobalBezierCamera(BezierCamera *bezierCamera);

BOOL bDoneAllInitialization = FALSE;
extern Camera camera;

class MainScene
{

public:
    // member variables
    int selected_scene;
    bool START_E2E_DEMO;

    // Scenes
    EarthBirthScene *earthBirthScene;
    EarthCooldownScene *earthCooldownScene;
    Terrain *terrain;

    // member functions
    MainScene()
    {
        // SCENE
        earthBirthScene = new EarthBirthScene();
        earthCooldownScene = new EarthCooldownScene();

        // Terrain
        terrain = new Terrain();

        // SCENE CONTROLS
        START_E2E_DEMO = false;
        selected_scene = SCENE_OPENING;
    }

    BOOL initialize()
    {
        // SCENES
        earthCooldownScene->initialize();
        // earthBirthScene->initialize();

        // if (START_E2E_DEMO)
        // {
        // }
        // else
        // {
        // switch (selected_scene)
        // {
        // case SCENE_OPENING:
        //     opening->initialize();
        //     break;
        // }
        // }

        return TRUE;
    }

    void display()
    {

        // earthBirthScene->display();
        // earthCooldownScene->display();
        // Terrain
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera.invertPitch();
		// camera.getEye()[1] -= 2 * (camera.getEye()[1] - 100.0f);
		
        terrain->updateTilesPositions();
        terrain->up = 1.0;
        terrain->draw();

        // displayScene();
        // switch (selected_scene)
        // {
        // case SCENE_OPENING:
        // {
        //     opening->display(0);
        // }
        // break;

        // default:
        //     break;
        // }
    }

    void update()
    {
        // earthBirthScene->update();
        earthCooldownScene->update();
        // SCENE SWITCHER
        // if (START_E2E_DEMO) // Switch scenes only if end to end demo is played
        // {

        // }

        // switch (selected_scene)
        // {
        // case SCENE_OPENING:
        //     opening->update();
        //     break;
        // }
    }
    void uninitialize()
    {
        // earthBirthScene->uninitialize();
        earthCooldownScene->uninitialize();
    }
};
