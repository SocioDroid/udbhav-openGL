#pragma once
#include "../../utils/common.h"
#include "../../effects/metaballARM/Metaball.h"
#include "../../effects/cubemap/Cubemap.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../effects/starfield/starfield.h"

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
    StarField *starfield;
    CubeMap *cubemap;

    // member functions
    MainScene()
    {
        metaball = new Metaball();
        starfield = new StarField();
        cubemap = new CubeMap();
        START_E2E_DEMO = false;
        selected_scene = SCENE_OPENING;
    }

    BOOL initialize()
    {
        starfield->initialize();
        metaball->initialize();
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
        // Cubemap
        pushMatrix(modelMatrix);
        {
            // modelMatrix = modelMatrix * vmath::translate(objX, objY, objZ);
            modelMatrix = modelMatrix * vmath::scale(1000.0f, 1000.0f, 1000.0f);
            cubemap->display();
        }
        modelMatrix = popMatrix();

        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * vmath::translate(0.0f, 0.0f, 8.0f);
            metaball->display();
        }
        modelMatrix = popMatrix();

        pushMatrix(modelMatrix);
        {
            starfield->display();
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
        if (starfield)
        {
            starfield->uninitialize();
            delete (starfield);
            starfield = NULL;
        }

        if (metaball)
        {
            metaball->uninitialize();
            delete (metaball);
            metaball = NULL;
        }
    }
};
