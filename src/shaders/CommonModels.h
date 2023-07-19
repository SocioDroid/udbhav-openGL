#pragma once
#include "../utils/model_using_assim/model.h"
#include "../utils/common.h"

class CommonModels
{
public:
    // INTRO
    Model *model_amc;
    Model *model_prastut;
    Model *model_bharatEkKhoj;

    CommonModels()
    {
        // INTRO
        model_amc = new Model();
        model_prastut = new Model();
        model_bharatEkKhoj = new Model();
    }

    // Member Functions
    BOOL initialize(int selected_scene, bool isE2E)
    {
        if (isE2E)
        {
            IntroModels();
        }
        else
        {
            switch (selected_scene)
            {
            case SCENE_01_EARTH_BIRTH:
                IntroModels();
                break;
            }
        }

        return TRUE;
    }

    // ================================= Scene Specific Loading
    void IntroModels()
    {
        model_amc->initializeModel(".\\assets\\models\\intro\\amc.obj");
        model_prastut->initializeModel(".\\assets\\models\\intro\\prastut.obj");
        model_bharatEkKhoj->initializeModel(".\\assets\\models\\intro\\bharatEkKhoj.obj");
    }

    void uninitialize(void)
    {
        if (model_amc)
        {
            model_amc->uninitialize();
            delete model_amc;
            model_amc = NULL;
        }
        if (model_prastut)
        {
            model_prastut->uninitialize();
            delete model_prastut;
            model_prastut = NULL;
        }
        if (model_bharatEkKhoj)
        {
            model_bharatEkKhoj->uninitialize();
            delete model_bharatEkKhoj;
            model_bharatEkKhoj = NULL;
        }
    }
};
