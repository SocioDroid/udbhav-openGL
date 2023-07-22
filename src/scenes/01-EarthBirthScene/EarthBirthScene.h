#pragma once
#include "../../utils/common.h"
#include "../../effects/metaballARM/Metaball.h"
#include "../../effects/cubemap/Cubemap.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../effects/starfield/starfield.h"
#include "../../shaders/bloom/HDR_Shader.h"
#include "../../effects/bloom/Bloom_Shaders.h"
#include "../../shaders/model/Model_Shader.h"
#include <random>

void setGlobalBezierCamera(BezierCamera *bezierCamera);
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> randDist(-1.0f, 1.0f);

class EarthBirthScene
{

public:
    bool isInitialized = false;
    // Scenes
    Metaball *metaball;
    StarField *starfield;
    CubeMap *cubemap;

    // Bloom
    BloomShaders bloomShader;
    GLuint fbo_UserMap; // FRAME BUFFER OBJECT
    GLuint rbo_UserMap; // RENDER BUFFER OBJECT
    GLuint fbo_texture_UserMap;

    // Intro models
    ModelShader intro_amc;
    ModelShader intro_prastut;
    ModelShader intro_bharatEkKhoj;

    float fadeAlpha = 1.0f;
    // Camera
    BezierCamera sceneCamera;
    std::vector<std::vector<float>> bezierPointsScene = {
        {2.500000f, -0.7f, 1.700000f},
        {2.500000f, -0.2f, 1.700000f},
        {2.500000f, 0.000001f, 1.700000f},
        {2.500000f, 0.399999f, 1.700000f},
        {2.500000f, 2.200000f, 1.600000f},
        {2.500000f, 2.699999f, 1.100000f},
        {2.500000f, 2.999999f, 1.200000f},
        {2.500000f, 3.199999f, 1.600000f},
        {2.500000f, 3.199999f, 1.100001f},
        {2.500000f, 3.399999f, 1.100000f},
        {2.500000f, 3.599998f, 1.500000f},
        {2.500000f, 3.599998f, 2.000000f},
        {2.500000f, 3.599998f, 2.400000f},
        {2.500000f, 3.599998f, 2.600000f},
        {2.500000f, 3.599998f, 3.000000f},
        {2.500000f, 3.599998f, 3.299999f},
        {2.500000f, 3.599998f, 3.699999f},
        {2.500000f, 3.599998f, 3.699999f},
        {2.500000f, 2.999999f, 3.999999f},
        {2.500000f, 3.499999f, 3.999999f},
        {2.500000f, 2.899999f, 4.099998f},
        {2.600000f, 3.199999f, 4.799998f},
        {2.500000f, 3.599998f, 4.799998f},
        {2.500000f, 2.799999f, 5.899997f},
        {2.500000f, 2.799999f, 6.999996f},
        {2.500000f, 2.799999f, 7.199996f},
        {2.500000f, 2.799999f, 7.199996f},
        {2.500000f, 2.799999f, 7.199996f},
        {2.500000f, 2.799999f, 7.199996f},

    };

    // YAW GLOBAL
    std::vector<float> yawScene = {
        90.000000f,
        90.000000f,
        90.000000f,
        90.000000f,
        90.000000f,
        90.000000f,
        90.000000f,
        90.000000f,
        90.000000f,
        90.000000f,
        90.000000f,
        90.000000f,
        90.000000f,
        90.000000f,
        90.000000f,
        70.000000f,
        -25.000000f,
        -43.000000f,
        -67.000000f,
        -90.000000f,
        -90.000000f,
        -90.000000f,
        -90.000000f,
        -90.000000f,
        -90.000000f,
        -90.000000f,
        -90.000000f,
        -90.000000f,
        -90.000000f,
    };

    // PITCH GLOBAL
    std::vector<float> pitchScene = {
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        -5.000000f,
        -12.000000f,
        -17.000000f,
        -21.000000f,
        -30.000000f,
        -50.000000f,
        -84.000000f,
        -89.000000f,
        -89.000000f,
        -82.000000f,
        -71.000000f,
        -64.000000f,
        -56.000000f,
        -55.000000f,
        -33.000000f,
        -28.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
        0.000000f,
    };

    // member functions
    EarthBirthScene()
    {
        metaball = new Metaball();
        starfield = new StarField();
        cubemap = new CubeMap();
    }

    BOOL initialize()
    {
        // Bloom
        bloomShader.initialize_bloomShaderObject();
        bloomShader.exposure = 10.0f;
        if (bloomShader.CreateSceneFBO(1920, 1080, fbo_UserMap, rbo_UserMap, fbo_texture_UserMap) == false)
        {
            PrintLog("Failed to create Scene FBO\n");
            return FALSE;
        }

        // Starfield
        starfield->initialize();

        // Metaball
        metaball->initialize();

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

        // Models
        intro_amc.initialize_ModelShaderObject(commonModels->model_amc);
        intro_prastut.initialize_ModelShaderObject(commonModels->model_prastut);
        intro_bharatEkKhoj.initialize_ModelShaderObject(commonModels->model_bharatEkKhoj);
        intro_amc.setLigthPosition(0.0f, 0.0f, 0.0f);
        intro_prastut.setLigthPosition(0.0f, 0.0f, 0.0f);
        intro_bharatEkKhoj.setLigthPosition(0.0f, 0.0f, 0.0f);

        // Camera
        sceneCamera.initialize();
        sceneCamera.setBezierPoints(bezierPointsScene, yawScene, pitchScene);
        sceneCamera.update();

        isInitialized = true;
        return TRUE;
    }

    float vibrationStrength = 1.0f; // Adjust this value to control the intensity of vibration

    // Global variables
    bool earthquakeActive = false;
    float earthquakeDuration = 7.0f; // Duration of earthquake in seconds
    float earthquakeStartTime = 0.0f;
    bool wasEarthquakeEnabled = false;

    // Function to simulate the earthquake jerk effect
    void earthquakeJerk(float timeElapsed)
    {
        if (earthquakeActive)
        {
            float earthquakeTime = timeElapsed - earthquakeStartTime;

            if (earthquakeTime <= earthquakeDuration)
            {
            }
            else
            {
                wasEarthquakeEnabled = true;
                earthquakeActive = false;
            }
        }
    }

    void startEarthquakeJerk(int value)
    {
        earthquakeActive = true;
        earthquakeStartTime = ELAPSED_TIME;
    }

    void display()
    {
        // Camera
        setGlobalBezierCamera(&sceneCamera);
        sceneCamera.update();
        updateGlobalViewMatrix();

        // Vibrate camera
        float currentTime = ELAPSED_TIME; // Convert milliseconds to seconds
        earthquakeJerk(currentTime);

        if (sceneCamera.time >= 1.0f)
            if (!earthquakeActive && !wasEarthquakeEnabled)
                startEarthquakeJerk(0);

        pushMatrix(modelMatrix);
        {
            // Earthquake
            if (earthquakeActive)
            {
                float offset = sin(20.1f * 3.14159265359f * ELAPSED_TIME);
                modelMatrix = modelMatrix * translate(offset * 0.003f, offset * 0.003f, 0.0f);
            }

            pushMatrix(modelMatrix);
            {
                glBindFramebuffer(GL_FRAMEBUFFER, fbo_UserMap);
                {
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    glViewport(0, 0, 1920, 1080);
                    displayScene();
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
            modelMatrix = popMatrix();

            pushMatrix(modelMatrix);
            {
                bloomShader.bindBloomFBO();
                {
                    // Applying bloom only to Metaballs
                    pushMatrix(modelMatrix);
                    {
                        modelMatrix = modelMatrix * scale(5.0f, 5.0f, 5.0f);
                        metaball->display();
                    }
                    modelMatrix = popMatrix();
                }
                bloomShader.unbindBloomFBO();

                bloomShader.renderBlurFBO();
                bloomShader.renderFinalBloomScene(fbo_texture_UserMap);
            }
            modelMatrix = popMatrix();
        }
        modelMatrix = popMatrix();

        // FADE IN
        pushMatrix(modelMatrix);
        {
            modelMatrix = vmath::scale(1.0f, 1.0f, 1.0f);
            commonShaders->overlayColorShader->draw(modelMatrix, 0.0f, 0.0f, 0.0f, fadeAlpha);
        }
        modelMatrix = popMatrix();
    }

    void displayScene()
    {
        // Cubemap
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * vmath::scale(1000.0f, 1000.0f, 1000.0f);
            cubemap->display();
        }
        modelMatrix = popMatrix();

        // Metaballs
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * scale(5.0f, 5.0f, 5.0f);
            metaball->display();
        }
        modelMatrix = popMatrix();

        // Starfield
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * vmath::translate(-5.999997f, -39.099976f, 200.102524f);
            starfield->display();
        }
        modelMatrix = popMatrix();

        // Intro Objects
        if (sceneCamera.time < 0.20f) // remove objects once sun is visible
        {
            /// ASTROMEDICOMP
            pushMatrix(modelMatrix);
            {
                modelMatrix = modelMatrix * vmath::translate(3.199999f, 0.200000f, 2.700000f) * rotate(180.0f, 0.0f, 1.0f, 0.0f) * scale(0.260000f, 0.260000f, 0.260000f);
                intro_amc.render_Model();
            }
            modelMatrix = popMatrix();
            /// Prastut karta hai
            pushMatrix(modelMatrix);
            {
                modelMatrix = modelMatrix * vmath::translate(3.199999f + -0.300000f, 0.20f + 1.100000f, 2.70f) * rotate(180.0f, 0.0f, 1.0f, 0.0f) * scale(0.260000f + 0.040000f, 0.260000f + 0.040000f, 0.260000f + 0.040000f);
                intro_prastut.render_Model();
            }
            modelMatrix = popMatrix();
        }

        if (sceneCamera.time > 0.3)
        {
            pushMatrix(modelMatrix);
            {
                modelMatrix = modelMatrix * vmath::translate(3.199999f + -2.300000f, 0.20f + 3.699999f, 2.70f) * scale(0.260000f + 1.069999f, 0.260000f + 1.069999f, 0.260000f + 1.069999f);
                intro_bharatEkKhoj.render_Model();
            }
            modelMatrix = popMatrix();
        }

        // Display bezier curve
        // sceneCamera.displayBezierCurve();
    }
    float camSpeed = 0.0001f;
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
        // Trigger fadeout
        if (ELAPSED_TIME > (START_TIME_SCENE_02_01_EARTH_COOLDOWN - 3))
        {
            isFadeout = true;
        }

        ////////////////////////////////// SCENE UPDATE
        if (sceneCamera.time <= 1.0f)
            sceneCamera.time += (0.000013f + camSpeed);
        else
        {
            // Starting metaball
            if (metaball->metaballTime < 2.0f)
            {
                metaball->metaballTime += 0.003f;
            }
        }
        if (sceneCamera.time > 0.17f)
        {
            if (camSpeed < 0.00058f)
                camSpeed += 0.0000013f;
        }

        metaball->update();
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

        if (cubemap)
        {
            cubemap->uninitialize();
            delete (cubemap);
            cubemap = NULL;
        }
    }
};

// TODO
// FADEIN , FADEOUT
// MAKE METABALL TEXCOORDS STABLE
