#pragma once
#include "../../utils/common.h"
#include "../../utils/camera/BezierCamera.h"

// SCENES
#include "../EarthBirthScene/EarthBirthScene.h"
#include "../EarthCooldownScene/EarthCooldownScene.h"

void setGlobalBezierCamera(BezierCamera *bezierCamera);

BOOL bDoneAllInitialization = FALSE;

class MainScene
{

public:
    // member variables
    int selected_scene;
    bool START_E2E_DEMO;

    // Scenes
    EarthBirthScene *earthBirthScene;
    EarthCooldownScene *earthCooldownScene;

    // member functions
    MainScene()
    {
        // SCENE
        earthBirthScene = new EarthBirthScene();
        earthCooldownScene = new EarthCooldownScene();

        // SCENE CONTROLS
        START_E2E_DEMO = false;
        selected_scene = SCENE_OPENING;
    }

    BOOL initialize()
    {
        // SCENES
        earthBirthScene->initialize();
        earthCooldownScene->initialize();

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
        earthCooldownScene->display();

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
