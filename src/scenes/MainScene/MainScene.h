#pragma once
#include "../../utils/common.h"
#include "../../utils/camera/BezierCamera.h"

// SCENES
#include "../MetaballScene/MetaballScene.h"

void setGlobalBezierCamera(BezierCamera *bezierCamera);

BOOL bDoneAllInitialization = FALSE;

class MainScene
{

public:
    // member variables
    int selected_scene;
    bool START_E2E_DEMO;

    // Scenes
    MetaballScene *metaballScene;

    // member functions
    MainScene()
    {
        // SCENE
        metaballScene = new MetaballScene();

        // SCENE CONTROLS
        START_E2E_DEMO = false;
        selected_scene = SCENE_OPENING;
    }

    BOOL initialize()
    {
        // SCENES
        metaballScene->initialize();

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

        metaballScene->display();

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
        metaballScene->update();
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
        metaballScene->uninitialize();
    }
};
