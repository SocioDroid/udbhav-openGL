#pragma once
#include <thread>
#include "../utils/model_using_assim/model.h"
#include "../utils/common.h"

class CommonModels
{
public:
    // Variables
    vector<std::thread> threads;

    // OPENING

    // INTRO
    Model *model_Gate;
    Model *model_Board;
    Model *model_Dog;
    Model *model_Bhikari;
    Model *model_vadga;

    // JIS_DESH
    Model *flagStick;
    Model *rajKapoor_JisDesh;
    Model *padmini_JisDesh;
    Model *daku1;
    Model *daku2;
    Model *daku3;
    Model *daku4;
    Model *cameraModel;
    Model *diffuseModel;
    Model *railModel;

    // BARSAT
    Model *boat;
    Model *BarsaatPoseNargis;
    Model *BarsaatPoseRK;
    Model *BarsatHouse;
    // Model *whiteTree;

    // SHREE420
    Model *children;
    Model *couple;
    Model *umbrella;
    Model *StreetWall;
    Model *tapriModel;
    Model *benchModel;
    Model *clapper;
    // Model *streetLightModel;
    Model *streetLightModel_lamp;
    Model *streetLightModel_poll;
    Model *table_brick;
    Model *Kettle;
    Model *CupSaucer;

    // SANGAM
    Model *roomSangam;
    Model *wineGlass;
    Model *wine;
    Model *rajKapoor_Sangam;
    Model *rajendraKumar_Sangam;
    Model *vyjayanthimala_Sangam;

    // AAG
    Model *Chotumastermodel;
    Model *stageAag;
    Model *nargis;
    Model *raj_Kapoor;
    Model *table;
    Model *mirrorFrame;

    // AWARA
    Model *awaraModel;
    Model *coupleModel;

    // STUDIO
    Model *stageStudio;
    Model *spotLight;
    Model *welcomeModel;
    Model *stoolModel;

    // OUTRO
    Model *model_Aishwarya;
    Model *model_Amogh;
    Model *model_Atharv;
    Model *model_Kishor;
    Model *model_Mrunal;
    Model *model_Pradnya;
    Model *model_Pratik;
    Model *model_RamaMadam;
    Model *model_Rutvik;
    Model *model_Sagar;
    Model *model_Sahil;
    Model *model_Varun;

    // MERA NAM JoKER
    Model *joker_fullbody;

    // LOGO
    Model *model_Logo;
    Model *model_Quad;

    CommonModels()
    {
        // OPENING

        // INTRO
        model_Gate = new Model();
        model_Board = new Model();
        model_Dog = new Model();
        model_Bhikari = new Model();
        model_vadga = new Model();

        // JIS_DESH
        flagStick = new Model();
        rajKapoor_JisDesh = new Model(true, 1000);
        padmini_JisDesh = new Model(true, 800);
        daku1 = new Model(true, 10);
        daku2 = new Model(true, 10);
        daku3 = new Model(true, 10);
        daku4 = new Model(true, 10);
        cameraModel = new Model();
        diffuseModel = new Model();
        railModel = new Model();

        // BARSAT
        boat = new Model();
        BarsaatPoseNargis = new Model(true, 1000);
        BarsaatPoseRK = new Model(true, 1000);
        BarsatHouse = new Model();
        // whiteTree = new Model();

        // SHREE420
        children = new Model(true, 800);
        couple = new Model(true, 1000);
        umbrella = new Model(); // NOT CONFIRMED FOR DisIntegration effect
        StreetWall = new Model();
        tapriModel = new Model();
        benchModel = new Model();
        clapper = new Model();
        // streetLightModel = new Model();
        streetLightModel_poll = new Model();
        streetLightModel_lamp = new Model();
        table_brick = new Model();
        Kettle = new Model();
        CupSaucer = new Model();

        // SANGAM
        roomSangam = new Model();
        wineGlass = new Model();
        wine = new Model();
        rajKapoor_Sangam = new Model(true, 5000);
        rajendraKumar_Sangam = new Model(true, 5000);
        vyjayanthimala_Sangam = new Model(true, 5000);

        // AAG
        Chotumastermodel = new Model(true, 3000);
        stageAag = new Model();
        nargis = new Model();
        raj_Kapoor = new Model(true, 7000);
        table = new Model();
        mirrorFrame = new Model();

        // AWARA
        awaraModel = new Model();
        coupleModel = new Model(true, 5000);

        // STUDIO
        stageStudio = new Model();
        welcomeModel = new Model;
        stoolModel = new Model();
        spotLight = new Model();

        // OUTRO
        model_Aishwarya = new Model(true, 10000);
        model_Amogh = new Model(true, 10000);
        model_Atharv = new Model(true, 10000);
        model_Kishor = new Model(true, 10000);
        model_Mrunal = new Model(true, 10000);
        model_Pradnya = new Model(true, 10000);
        model_Pratik = new Model(true, 10000);
        model_RamaMadam = new Model(true, 10000);
        model_Rutvik = new Model(true, 10000);
        model_Sagar = new Model(true, 10000);
        model_Sahil = new Model(true, 10000);
        model_Varun = new Model(true, 10000);

        // MERA NAM JOKER
        joker_fullbody = new Model(true, 20000);

        // LOGO
        model_Logo = new Model();
        model_Quad = new Model();
    }

    // Member Functions
    BOOL initialize(int selected_scene, bool isE2E)
    {
        if (isE2E)
        {
            IntroModels();
            JisDeshModels();
            BarsatModels();
            Shree420Models();
            SangamModels();
            AagModels();
            AwaraModels();
            StudioModels();
            MeraNamJokerModels();
            OutroModels();
            // threads.push_back(thread(&CommonModels::Batch1, this));
            // threads.push_back(thread(&CommonModels::Batch2, this));
            // threads.push_back(thread(&CommonModels::Batch3, this));
        }
        else
        {
            switch (selected_scene)
            {
            case SCENE_OPENING:
                break;
            }
        }
        CommonModels_();

        // Wait for threads
        // for (auto &th : threads)
        //     th.join();

        unsigned int n = std::thread::hardware_concurrency();
        PrintLog("Max Threads = %d", n);
        return TRUE;
    }

    // ================================= MultiThreading Functions
    void Batch1()
    {
        flagStick->initializeModel(".\\assets\\models\\FlagStick\\FlagStick.obj");
        rajKapoor_JisDesh->initializeModel(".\\assets\\models\\JisDeshMein\\RK.obj");
        padmini_JisDesh->initializeModel(".\\assets\\models\\JisDeshMein\\Padmini.obj");
        daku1->initializeModel(".\\assets\\models\\JisDeshMein\\Daku1 Decimated.obj");
        daku2->initializeModel(".\\assets\\models\\JisDeshMein\\Daku2 Decimated.obj");
        daku3->initializeModel(".\\assets\\models\\JisDeshMein\\Daku3 Decimated.obj");
        daku4->initializeModel(".\\assets\\models\\JisDeshMein\\Daku4 Decimated.obj");
        cameraModel->initializeModel(".\\assets\\models\\JisDeshMein\\Camera.obj");
        diffuseModel->initializeModel(".\\assets\\models\\JisDeshMein\\Diffuser.obj");
        railModel->initializeModel(".\\assets\\models\\JisDeshMein\\CameraRails.obj");
        model_Gate->initializeModel(".\\assets\\models\\Gate\\GATE\\Gate.obj");
        model_Board->initializeModel(".\\assets\\models\\SaleBoard\\ForSaleBoard.obj");
        model_Dog->initializeModel(".\\assets\\models\\Gate\\Dog\\Dog.obj");
        model_Bhikari->initializeModel(".\\assets\\models\\Gate\\Bhikari with red colour model on his head.obj");
        model_vadga->initializeModel(".\\assets\\models\\Gate\\vadga.obj");

        Chotumastermodel->initializeModel(".\\assets\\models\\Aag\\Chotumastermodel.obj");
        stageAag->initializeModel(".\\assets\\models\\Aag\\KewalTheatre.obj");
        nargis->initializeModel(".\\assets\\models\\Aag\\nargis_final_pose.obj");
        raj_Kapoor->initializeModel(".\\assets\\models\\Aag\\Raj_kapoor_final2.obj");
        table->initializeModel(".\\assets\\models\\Aag\\table\\Table.obj");
        children->initializeModel(".\\assets\\models\\Shree420\\models\\children.obj");
        couple->initializeModel(".\\assets\\models\\Shree420\\models\\couple-old.obj");
        umbrella->initializeModel(".\\assets\\models\\Shree420\\models\\umbrella.obj");
        StreetWall->initializeModel(".\\assets\\models\\Shree420\\RoadWall\\RoadSideWall.obj");
    }

    void Batch2()
    {
        BarsaatPoseNargis->initializeModel(".\\assets\\models\\Barsat\\Pose\\BarsaatPoseNargis.obj");
        BarsaatPoseRK->initializeModel(".\\assets\\models\\Barsat\\Pose\\BarsaatPoseRK.obj");
        BarsatHouse->initializeModel(".\\assets\\models\\Barsat\\House\\BarsaatHouse.obj");
        // whiteTree->initializeModel(".\\assets\\models\\Barsat\\Tree\\Tree.obj");
        awaraModel->initializeModel(".\\assets\\models\\Awara\\awaraModelRotated.obj");
        coupleModel->initializeModel(".\\assets\\models\\Awara\\AwaraModels.obj");

        roomSangam->initializeModel(".\\assets\\models\\sangamRoom\\SangamRoomScene.obj");
        wineGlass->initializeModel(".\\assets\\models\\WineGlass\\AlphaWineglass.obj");
        wine->initializeModel(".\\assets\\models\\WineGlass\\AlphaWine.obj");
        rajKapoor_Sangam->initializeModel(".\\assets\\models\\sangamRoom\\RK.obj");
        rajendraKumar_Sangam->initializeModel(".\\assets\\models\\sangamRoom\\RajendraKumar.obj");
        vyjayanthimala_Sangam->initializeModel(".\\assets\\models\\sangamRoom\\Vyjayanthimala.obj");

        stageStudio->initializeModel(".\\assets\\models\\ShootingStage-new\\ShootingStage.obj");
        spotLight->initializeModel(".\\assets\\models\\Studio\\SpotLight\\SpotLight.obj");
        welcomeModel->initializeModel(".\\assets\\models\\Welcome\\Welcome2.obj");
        stoolModel->initializeModel(".\\assets\\models\\Welcome\\table.obj");
    }

    void Batch3()
    {
        tapriModel->initializeModel(".\\assets\\models\\Shree420\\Tapri\\TapriModel.obj");
        benchModel->initializeModel(".\\assets\\models\\Shree420\\Bench\\StreetBench.obj");
        clapper->initializeModel(".\\assets\\models\\Shree420\\Clapper\\ClapperBoard.obj");
        // streetLightModel->initializeModel(".\\assets\\models\\Shree420\\StreetLight\\classic single light.obj");
        streetLightModel_poll->initializeModel(".\\assets\\models\\Shree420\\StreetLight\\justPoll.obj");
        streetLightModel_lamp->initializeModel(".\\assets\\models\\Shree420\\StreetLight\\justLamp.obj");
        table_brick->initializeModel(".\\assets\\models\\Shree420\\Brick\\Brick.obj");
        Kettle->initializeModel(".\\assets\\models\\Shree420\\Teapot\\teapot.obj");
        CupSaucer->initializeModel(".\\assets\\models\\Shree420\\CupSaucer\\CupAndSaucer.obj");
        model_Aishwarya->initializeModel(".\\assets\\models\\Outro\\Aishwarya.obj");
        model_Amogh->initializeModel(".\\assets\\models\\Outro\\Amogh.obj");
        model_Atharv->initializeModel(".\\assets\\models\\Outro\\Atharv.obj");
        model_Kishor->initializeModel(".\\assets\\models\\Outro\\Kishor.obj");
        model_Mrunal->initializeModel(".\\assets\\models\\Outro\\Mrunal.obj");
        model_Pradnya->initializeModel(".\\assets\\models\\Outro\\Pradnya.obj");
        model_Pratik->initializeModel(".\\assets\\models\\Outro\\Pratik.obj");
        model_RamaMadam->initializeModel(".\\assets\\models\\Outro\\RamaMadam.obj");
        model_Rutvik->initializeModel(".\\assets\\models\\Outro\\Rutvik.obj");
        model_Sagar->initializeModel(".\\assets\\models\\Outro\\Sagar.obj");
        model_Sahil->initializeModel(".\\assets\\models\\Outro\\Sahil.obj");
        model_Varun->initializeModel(".\\assets\\models\\Outro\\Varun.obj");
        mirrorFrame->initializeModel(".\\assets\\models\\Aag\\Mirror\\mirror.obj");
        boat->initializeModel(".\\assets\\models\\Boat\\boat.obj");
        model_Logo->initializeModel(".\\assets\\models\\Logo\\MatrixLogo.obj");
        model_Quad->initializeModel(".\\assets\\models\\Logo\\Quad.obj");
    }
    // ===========================================================

    // ================================= Scene Specific Loading
    void IntroModels()
    {
        model_Gate->initializeModel(".\\assets\\models\\Gate\\Gate.obj");
        model_Board->initializeModel(".\\assets\\models\\SaleBoard\\ForSaleBoard.obj");
        model_Dog->initializeModel(".\\assets\\models\\Gate\\Dog\\Dog.obj");
        model_Bhikari->initializeModel(".\\assets\\models\\Gate\\Bhikari with red colour model on his head.obj");
        model_vadga->initializeModel(".\\assets\\models\\Gate\\vadga.obj");
    }

    void JisDeshModels()
    {
        flagStick->initializeModel(".\\assets\\models\\FlagStick\\FlagStick.obj");
        rajKapoor_JisDesh->initializeModel(".\\assets\\models\\JisDeshMein\\RK.obj");
        padmini_JisDesh->initializeModel(".\\assets\\models\\JisDeshMein\\Padmini.obj");
        daku1->initializeModel(".\\assets\\models\\JisDeshMein\\Daku1.obj");
        daku2->initializeModel(".\\assets\\models\\JisDeshMein\\Daku2.obj");
        daku3->initializeModel(".\\assets\\models\\JisDeshMein\\Daku3.obj");
        daku4->initializeModel(".\\assets\\models\\JisDeshMein\\Daku4.obj");
        railModel->initializeModel(".\\assets\\models\\JisDeshMein\\CameraRails.obj");
    }
    void BarsatModels()
    {
        BarsaatPoseNargis->initializeModel(".\\assets\\models\\Barsat\\Pose\\BarsaatPoseNargis.obj");
        BarsaatPoseRK->initializeModel(".\\assets\\models\\Barsat\\Pose\\BarsaatPoseRK.obj");
        BarsatHouse->initializeModel(".\\assets\\models\\Barsat\\House\\BarsaatHouse.obj");
        // whiteTree->initializeModel(".\\assets\\models\\Barsat\\Tree\\Tree.obj");
    }
    void Shree420Models()
    {
        children->initializeModel(".\\assets\\models\\Shree420\\models\\children.obj");
        couple->initializeModel(".\\assets\\models\\Shree420\\models\\couple-old.obj");
        umbrella->initializeModel(".\\assets\\models\\Shree420\\models\\umbrella.obj");
        StreetWall->initializeModel(".\\assets\\models\\Shree420\\RoadWall\\RoadSideWall.obj");
        tapriModel->initializeModel(".\\assets\\models\\Shree420\\Tapri\\TapriModel.obj");
        benchModel->initializeModel(".\\assets\\models\\Shree420\\Bench\\StreetBench.obj");
        clapper->initializeModel(".\\assets\\models\\Shree420\\Clapper\\ClapperBoard.obj");

        // streetLightModel->initializeModel(".\\assets\\models\\Shree420\\StreetLight\\classic single light.obj");
        streetLightModel_poll->initializeModel(".\\assets\\models\\Shree420\\StreetLight\\justPoll.obj");
        streetLightModel_lamp->initializeModel(".\\assets\\models\\Shree420\\StreetLight\\justLamp.obj");
        table_brick->initializeModel(".\\assets\\models\\Shree420\\Brick\\Brick.obj");
        Kettle->initializeModel(".\\assets\\models\\Shree420\\Teapot\\teapot.obj");
        CupSaucer->initializeModel(".\\assets\\models\\Shree420\\CupSaucer\\CupAndSaucer.obj");
        // threads.push_back(thread(&Model::initializeModel, children, ".\\assets\\models\\Shree420\\models\\children.obj"));
        // threads.push_back(thread(&Model::initializeModel, couple, ".\\assets\\models\\Shree420\\models\\couple-old.obj"));
        // threads.push_back(thread(&Model::initializeModel, umbrella, ".\\assets\\models\\Shree420\\models\\umbrella.obj"));
        // threads.push_back(thread(&Model::initializeModel, StreetWall, ".\\assets\\models\\Shree420\\RoadWall\\RoadSideWall.obj"));
        // threads.push_back(thread(&Model::initializeModel, tapriModel, ".\\assets\\models\\Shree420\\Tapri\\TapriModel.obj"));
        // threads.push_back(thread(&Model::initializeModel, benchModel, ".\\assets\\models\\Shree420\\Bench\\StreetBench.obj"));
        // threads.push_back(thread(&Model::initializeModel, streetLightModel, ".\\assets\\models\\Shree420\\StreetLight\\classic single light.obj"));
        // threads.push_back(thread(&Model::initializeModel, streetLightModel_poll, ".\\assets\\models\\Shree420\\StreetLight\\justPoll.obj"));
        // threads.push_back(thread(&Model::initializeModel, streetLightModel_lamp, ".\\assets\\models\\Shree420\\StreetLight\\justLamp.obj"));
        // threads.push_back(thread(&Model::initializeModel, table_brick, ".\\assets\\models\\Shree420\\Brick\\Brick.obj"));
        // threads.push_back(thread(&Model::initializeModel, Kettle, ".\\assets\\models\\Shree420\\Teapot\\teapot.obj"));
        // threads.push_back(thread(&Model::initializeModel, CupSaucer, ".\\assets\\models\\Shree420\\CupSaucer\\CupAndSaucer.obj"));
    }
    void SangamModels()
    {
        roomSangam->initializeModel(".\\assets\\models\\sangamRoom\\SangamRoomScene.obj");
        wineGlass->initializeModel(".\\assets\\models\\WineGlass\\AlphaWineglass.obj");
        wine->initializeModel(".\\assets\\models\\WineGlass\\AlphaWine.obj");
        rajKapoor_Sangam->initializeModel(".\\assets\\models\\sangamRoom\\rksangam_NEW.obj");
        rajendraKumar_Sangam->initializeModel(".\\assets\\models\\sangamRoom\\Rangendra_NEW.obj");
        vyjayanthimala_Sangam->initializeModel(".\\assets\\models\\sangamRoom\\VijayantiMalaDecimated_NEW.obj");
    }

    void AagModels()
    {
        Chotumastermodel->initializeModel(".\\assets\\models\\Aag\\Chotumastermodel.obj");
        nargis->initializeModel(".\\assets\\models\\Aag\\nargis_final_pose.obj");
        raj_Kapoor->initializeModel(".\\assets\\models\\Aag\\Raj_kapoor_final2.obj");
        stageAag->initializeModel(".\\assets\\models\\Aag\\KewalTheatre.obj");
        table->initializeModel(".\\assets\\models\\Aag\\table\\Table.obj");
    }
    void AwaraModels()
    {
        awaraModel->initializeModel(".\\assets\\models\\Awara\\awaraModelRotated.obj");
        coupleModel->initializeModel(".\\assets\\models\\Awara\\AwaraModels.obj");
    }
    void StudioModels()
    {
        stageStudio->initializeModel(".\\assets\\models\\ShootingStage-new\\ShootingStage.obj");
        welcomeModel->initializeModel(".\\assets\\models\\Welcome\\Welcome2.obj");
        stoolModel->initializeModel(".\\assets\\models\\Welcome\\table.obj");
    }

    void MeraNamJokerModels()
    {
        joker_fullbody->initializeModel(".\\assets\\models\\MeraNamJoker\\joker_final_draft.obj");
    }

    void OutroModels()
    {
        model_Aishwarya->initializeModel(".\\assets\\models\\Outro\\Aishwarya.obj");
        model_Amogh->initializeModel(".\\assets\\models\\Outro\\Amogh.obj");
        model_Atharv->initializeModel(".\\assets\\models\\Outro\\Atharv.obj");
        model_Kishor->initializeModel(".\\assets\\models\\Outro\\Kishor.obj");
        model_Mrunal->initializeModel(".\\assets\\models\\Outro\\Mrunal.obj");
        model_Pradnya->initializeModel(".\\assets\\models\\Outro\\Pradnya.obj");
        model_Pratik->initializeModel(".\\assets\\models\\Outro\\Pratik.obj");
        model_RamaMadam->initializeModel(".\\assets\\models\\Outro\\RamaMadam.obj");
        model_Rutvik->initializeModel(".\\assets\\models\\Outro\\Rutvik.obj");
        model_Sagar->initializeModel(".\\assets\\models\\Outro\\Sagar.obj");
        model_Sahil->initializeModel(".\\assets\\models\\Outro\\Sahil.obj");
        model_Varun->initializeModel(".\\assets\\models\\Outro\\Varun.obj");
    }
    void logoModels()
    {
        model_Logo->initializeModel(".\\assets\\models\\Logo\\MatrixLogo.obj");
        model_Quad->initializeModel(".\\assets\\models\\Logo\\Quad.obj");
    }

    void CommonModels_()
    {
        mirrorFrame->initializeModel(".\\assets\\models\\Aag\\Mirror\\mirror.obj");
        boat->initializeModel(".\\assets\\models\\Boat\\boat.obj");
        spotLight->initializeModel(".\\assets\\models\\Studio\\SpotLight\\SpotLight.obj");
        cameraModel->initializeModel(".\\assets\\models\\JisDeshMein\\Camera.obj");
        diffuseModel->initializeModel(".\\assets\\models\\JisDeshMein\\Diffuser.obj");
    }
    void releaseHeavyModels()
    {
        rajKapoor_JisDesh->uninitialize();
        padmini_JisDesh->uninitialize();
        daku1->uninitialize();
        daku2->uninitialize();
        daku3->uninitialize();
        daku4->uninitialize();
        model_Gate->uninitialize();
        model_Board->uninitialize();
        model_Dog->uninitialize();
        model_Bhikari->uninitialize();
        model_vadga->uninitialize();
        BarsaatPoseNargis->uninitialize();
        BarsaatPoseRK->uninitialize();
        children->uninitialize();
        couple->uninitialize();
        rajKapoor_Sangam->uninitialize();
        rajendraKumar_Sangam->uninitialize();
        vyjayanthimala_Sangam->uninitialize();
        roomSangam->uninitialize();
    }
    void uninitialize(void)
    {
        if (joker_fullbody)
        {
            delete joker_fullbody;
            joker_fullbody = NULL;
        }

        if (padmini_JisDesh)
        {
            delete padmini_JisDesh;
            padmini_JisDesh = NULL;
        }

        if (rajKapoor_JisDesh)
        {
            delete rajKapoor_JisDesh;
            rajKapoor_JisDesh = NULL;
        }

        if (vyjayanthimala_Sangam)
        {
            delete vyjayanthimala_Sangam;
            vyjayanthimala_Sangam = NULL;
        }

        if (rajendraKumar_Sangam)
        {
            delete rajendraKumar_Sangam;
            rajendraKumar_Sangam = NULL;
        }

        if (rajKapoor_Sangam)
        {
            delete rajKapoor_Sangam;
            rajKapoor_Sangam = NULL;
        }

        if (BarsaatPoseRK)
        {
            delete BarsaatPoseRK;
            BarsaatPoseRK = NULL;
        }

        if (BarsaatPoseNargis)
        {
            delete BarsaatPoseNargis;
            BarsaatPoseNargis = NULL;
        }

        if (Chotumastermodel)
        {
            delete Chotumastermodel;
            Chotumastermodel = NULL;
        }
    }
};
