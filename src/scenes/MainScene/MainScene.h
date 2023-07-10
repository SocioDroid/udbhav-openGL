#pragma once
#include "../../utils/common.h"
#include "../../effects/metaballARM/Metaball.h"
#include "../../utils/camera/BezierCamera.h"

void setGlobalBezierCamera(BezierCamera *bezierCamera);

BOOL bDoneAllInitialization = FALSE;

class MainScene
{

public:
    // member variables
    int selected_scene;
    bool START_E2E_DEMO;

    // Scenes
    Metaball *metaball;

    // member functions
    MainScene()
    {
        metaball = new Metaball();

        START_E2E_DEMO = false;
        selected_scene = SCENE_OPENING;
    }

    BOOL initialize()
    {
        metaball->initialize();
        // if (START_E2E_DEMO)
        // {
        //     opening->initialize();
        // }
        // else
        // {
        //     switch (selected_scene)
        //     {
        //     case SCENE_OPENING:
        //         opening->initialize();
        //         break;
        //     }
        // }

        return TRUE;
    }

    void display()
    {
        pushMatrix(modelMatrix);
        {
            metaball->display();
        }
        modelMatrix = popMatrix();
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
        metaball->update();
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
        if (metaball)
        {
            metaball->uninitialize();
            delete (metaball);
            metaball = NULL;
        }
    }
};
