#pragma once
#include "../../utils/common.h"
#include "../../effects/cubemap/Cubemap.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../includes/SphereAish.h"
#include "../../shaders/terrainPostProcess/TerrainPostProcess.h"
#include "../../shaders/shadow/ShadowShader.h"
#include "../../effects/terrain/Terrain.h"
#include "../../effects/water_matrix/WaterMatrix.h"
#include "../../utils/camera/BezierCamera.h"

class TerrainShadowScene
{

public:
    bool isInitialized = false;
    Terrain *terrain;
    CubeMap *cubemap;
    WaterMatrix *waterMatrix;
    BezierCamera sceneCamera;
    SphereAish *shadowSphere;
    ShadowShader shadowShader;

    ModelShader model_tree;
    ModelShader model_treeGroup2;
    ModelShader model_treeSingle;

    // Fadein Fadeout
    float fadeAlpha = 1.0f;
    float scaleFactor = 2.0f;

    // shadow
    float maxShadowTranslate = 158.0f;
    float shadowTranslate = 0.0f;

    std::vector<std::vector<float>> bezierPointsScene = {
        {4800.000000f, 1950.000000f, 43950.000000f},
        {4800.000000f, 1950.000000f, 43950.000000f},
        {4800.000000f, 1950.000000f, 43950.000000f},
        {4800.000000f, 2550.000000f, 43950.000000f},
        {5850.000000f, 2550.000000f, 43050.000000f},
        {5850.000000f, 2550.000000f, 40650.000000f},
        {6900.000000f, 2550.000000f, 38400.000000f},
        {7500.000000f, 2550.000000f, 36300.000000f},
        {11100.000000f, 2550.000000f, 32100.000000f},
        {15300.000000f, 3300.000000f, 28200.000000f},
        {15300.000000f, 3300.000000f, 25950.000000f},
        {17100.000000f, 3300.000000f, 23400.000000f},
        {17100.000000f, 3300.000000f, 21600.000000f},
        {17100.000000f, 2850.000000f, 17700.000000f},
        {13650.000000f, 2850.000000f, 13500.000000f},
        {11550.000000f, 3450.000000f, 11100.000000f},
        {9600.000000f, 3600.000000f, 9000.000000f},
        {7050.000000f, 3450.000000f, 2850.000000f},
        {4650.000000f, 2850.000000f, 450.000000f},
        {3150.000000f, 2700.000000f, -450.000000f},
        {3000.000000f, 2850.000000f, -600.000000f},
        {3000.000000f, 3000.000000f, -750.000000f},
        {3000.000000f, 3150.000000f, -750.000000f},
        {3000.000000f, 3450.000000f, -750.000000f},
        {3000.000000f, 3750.000000f, -750.000000f},
        {3000.000000f, 4200.000000f, -750.000000f},
        {3000.000000f, 4800.000000f, -750.000000f},
        {3000.000000f, 6000.000000f, -750.000000f},
        {3000.000000f, 5700.000000f, -750.000000f},
        {3000.000000f, 5700.000000f, -750.000000f},
    };

    // YAW GLOBAL
    std::vector<float> yawScene = {
        120.000000f,
        120.000000f,
        120.000000f,
        120.000000f,
        120.000000f,
        120.000000f,
        120.000000f,
        120.000000f,
        120.000000f,
        119.000000f,
        119.000000f,
        106.000000f,
        98.000000f,
        92.000000f,
        87.000000f,
        61.000000f,
        61.000000f,
        57.000000f,
        63.000000f,
        72.000000f,
        65.000000f,
        65.000000f,
        65.000000f,
        65.000000f,
        65.000000f,
        65.000000f,
        65.000000f,
        65.000000f,
        65.000000f,
        65.000000f,
        65.000000f,
    };

    // PITCH GScene
    std::vector<float> pitchScene = {
        -67.000000f,
        -67.000000f,
        -67.000000f,
        -67.000000f,
        -51.000000f,
        -34.000000f,
        -22.000000f,
        -16.000000f,
        -16.000000f,
        -19.000000f,
        -19.000000f,
        -16.000000f,
        -13.000000f,
        -16.000000f,
        -10.000000f,
        -10.000000f,
        -13.000000f,
        -13.000000f,
        -11.000000f,
        -11.000000f,
        -11.000000f,
        -11.000000f,
        -11.000000f,
        -11.000000f,
        -11.000000f,
        -11.000000f,
        -15.000000f,
        -15.000000f,
        -19.000000f,
        -20.000000f,
        -20.000000f,
    };

    // member functions
    TerrainShadowScene()
    {
        // Terrain
        terrain = new Terrain(20. * 100.);
        terrain->shadowLightX = -9.199999f;
        terrain->shadowLightY = 0.699999f;
        terrain->shadowLightZ = -4.999997f;
        cubemap = new CubeMap();
        waterMatrix = new WaterMatrix(1000. * 70.);
    }

    BOOL initialize()
    {

        // Cubemap
        const char *facesLight[] =
            {
                "./assets/textures/terrain/cubemap_light/px.png",
                "./assets/textures/terrain/cubemap_light/nx.png",
                "./assets/textures/terrain/cubemap_light/py.png",
                "./assets/textures/terrain/cubemap_light/ny.png",
                "./assets/textures/terrain/cubemap_light/pz.png",
                "./assets/textures/terrain/cubemap_light/nz.png"};

        if (!cubemap->initialize(facesLight))
        {
            PrintLog("Failed to initialize CubeMap");
            return FALSE;
        }

        // Water
        waterMatrix->initialize();

        // Shadow Initialization
        shadowShader.initialize();
        sceneCamera.setBezierPoints(bezierPointsScene, yawScene, pitchScene);
        shadowSphere = new SphereAish(1000.0f, 50, 50);

        // Trees
        model_tree.initialize_ModelShaderObject(commonModels->model_tree);
        model_treeGroup2.initialize_ModelShaderObject(commonModels->model_treeGroup2);
        model_treeSingle.initialize_ModelShaderObject(commonModels->model_treeSingle);

        // Camera
        sceneCamera.initialize();
        sceneCamera.isWater = true;
        isInitialized = true;
        return TRUE;
    }

    void display()
    {
        // // Camera
        modelMatrix = mat4::identity();
        viewMatrix = mat4::identity();

        setGlobalBezierCamera(&sceneCamera);
        // sceneCamera.setBezierPoints(bezierPoints, yawGlobal, pitchGlobal);
        sceneCamera.update();
        updateGlobalViewMatrix();

        // Water FBO
        {
            // Reflection
            waterMatrix->bindReflectionFBO(1920, 1080);
            {
                displayScene(1.0, false);
            }
            waterMatrix->unbindReflectionFBO();

            // Refraction
            waterMatrix->bindRefractionFBO(1920, 1080);
            {
                displayScene(-1.0, false);
            }
            waterMatrix->unbindRefractionFBO();
        }

        // Actual Scene
        displayScene(1.0, true);

        // // Water Bed
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * translate(0.0f + 800.000000f, -56.900028f, 0.0f + 63300.000000f);
            waterMatrix->renderWaterQuad(terrain->getWaterHeight());
        }
        modelMatrix = popMatrix();

        // cubemap
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * vmath::scale(500000.0f, 500000.0f, 500000.0f);
            cubemap->display();
        }
        modelMatrix = popMatrix();

        // displayShadowObject(false);

        // FADE IN
        pushMatrix(modelMatrix);
        {
            modelMatrix = vmath::scale(1.0f, 1.0f, 1.0f);
            commonShaders->overlayColorShader->draw(modelMatrix, 0.0f, 0.0f, 0.0f, fadeAlpha);
        }
        modelMatrix = popMatrix();

        // displayShadow();
        // sceneCamera.displayBezierCurve();
    }

    void displayShadow()
    {
        mat4 lightProjectionMatrix = mat4::identity();
        mat4 lightViewMatrix = mat4::identity();
        mat4 lightSpaceMatrix = mat4::identity();
        float near_plane = 1.0f, far_plane = 0.0f;

        shadowShader.lightPos[0] = 1.600000f + 2.000000f;
        shadowShader.lightPos[1] = 5.599997f + shadowTranslate;
        shadowShader.lightPos[2] = 4.599998f;

        shadowShader.lightDirection[0] = 0.0f;
        shadowShader.lightDirection[1] = 0.0f;
        shadowShader.lightDirection[2] = 0.0f;

        // PrintLog("%f, %f, %f\n%f, %f, %f\n\n", lightPos[0], lightPos[1], lightPos[2], lightDirection[0], lightDirection[1], lightDirection[2]);
        lightProjectionMatrix = vmath::ortho(-1000000.0f, 1000000.0f, -1000000.0f, 1000000.0f, 1.0f, far_plane + 10000.0f);
        lightViewMatrix = vmath::lookat(vec3(shadowShader.lightPos[0], shadowShader.lightPos[1], shadowShader.lightPos[2]), vec3(shadowShader.lightDirection[0], shadowShader.lightDirection[1], shadowShader.lightDirection[2]), vec3(0.0, 1.0, 0.0));
        // lightViewMatrix = vmath::lookat(camera.getEye(), camera.getCenter(), vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use Depth Shader
        glUseProgram(shadowShader.shaderProgramObject_depth);
        {
            glViewport(0, 0, shadowShader.SHADOW_WIDTH, shadowShader.SHADOW_HEIGHT);

            glBindFramebuffer(GL_FRAMEBUFFER, shadowShader.depthMapFBO);
            {
                glUniformMatrix4fv(shadowShader.projectionMatrixUniform_depth, 1, GL_FALSE, lightSpaceMatrix);
                // glUniformMatrix4fv(viewMatrixUniform_depth, 1, GL_FALSE, lightViewMatrix);
                mat4 translateMatrix = mat4::identity();
                mat4 rotateMatrix = mat4::identity();
                modelMatrix = translate(1890.000000f, 5950.000000f, 8190.000000f) * scale(165.000000f, 1.0f, 165.000000f);
                glUniformMatrix4fv(shadowShader.modelMatrixUniform_depth, 1, GL_FALSE, modelMatrix);

                glClear(GL_DEPTH_BUFFER_BIT);

                displayShadowObject(true);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        glUseProgram(0);

        // Render the scene using the generated depth/shadow map
        {
            modelMatrix = mat4::identity();

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glViewport(0, 0, giWindowWidth, giWindowHeight);

            perspectiveProjectionMatrix = vmath::perspective(
                45.0f,
                (GLfloat)giWindowWidth / (GLfloat)giWindowHeight,
                0.1f,
                10000000.0f);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // terrain->shadowLightPosition = camera.getEye();
            terrain->shadowLightPosition = vec3(shadowShader.lightPos[0], shadowShader.lightPos[1], shadowShader.lightPos[2]);
            terrain->shadowLightSpaceMatrix = lightSpaceMatrix;
            terrain->shadowMap = shadowShader.depthMapTexture;
            terrain->draw(true);
        }

        // pushMatrix(modelMatrix);
        // {
        //     modelMatrix = translate(objX, objY, objZ) * scale(light_objY, 0.1f, light_objY);
        //     displayShadowObject(false);
        // }
        // modelMatrix = popMatrix();

        // // USE DEBUG QUAD
        // glUseProgram(shadowShader.shaderProgramObject_debugQuad);
        // {
        //     glViewport(0, 0, giWindowWidth, giWindowHeight);
        //     glUniform1f(shadowShader.nearPlaneUniform_debudQuad, 1.0f);
        //     glUniform1f(shadowShader.farPlaneUniform_debudQuad, far_plane + 10000.0f);
        //     glActiveTexture(GL_TEXTURE0);
        //     glBindTexture(GL_TEXTURE_2D, shadowShader.depthMapTexture);
        //     shadowShader.renderQuad();
        // }
        // glUseProgram(0);
    }

    void displayScene(float terrainUp, bool isShadow)
    {
        // cubemap
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * vmath::scale(500000.0f, 500000.0f, 500000.0f);
            cubemap->display();
        }
        modelMatrix = popMatrix();

        // Terrain
        pushMatrix(modelMatrix);
        {
            terrain->up = terrainUp;
            if (isShadow)
                displayShadow();
            else
                terrain->draw(true);
        }
        modelMatrix = popMatrix();

        // Trees

        drawPineTrees();
    }

    void drawPineTrees()
    {
        std::vector<std::vector<float>> bezierPointsGroupSmall = {
            {-0.200000f, -4.699998f, 18.200033f},
            {-2.000000f, -4.699998f, 18.300034f},
            {-2.000000f, -4.699998f, 16.800028f},
            {-3.399999f, -4.699998f, 17.900032f},
            {-3.399999f, -4.699998f, 16.100025f},
            {-4.899998f, -4.699998f, 16.300026f},
            {-4.899998f, -4.699998f, 14.700020f},
            {-4.899998f, -4.699998f, 13.100014f},
            {-6.399996f, -4.699998f, 13.100014f},
            {-8.499996f, -4.699998f, 13.100014f},
            {-8.499996f, -4.699998f, 14.400019f},
            {-6.799996f, -4.699998f, 14.400019f},
            {-6.699996f, -4.699998f, 11.500008f},
            {-8.099995f, -4.699998f, 11.500008f},
            {-9.199999f, -4.699998f, 11.500008f},
            {-9.299999f, -4.699998f, 10.300003f},
            {-9.299999f, -4.699998f, 8.699997f},
            {-9.299999f, -4.699998f, 6.999996f},
            {-10.299999f, -4.699998f, 4.999996f},
            {-8.299999f, -4.699998f, 4.999996f},
            {-8.299999f, -4.699998f, 3.999996f},
            {-10.299999f, -4.699998f, 3.999996f},
            {-11.299999f, -4.699998f, 1.999996f},
            {-9.299999f, -4.699998f, 1.999996f},
            {-9.299999f, -3.699998f, -1.000004f},
            {-11.299999f, -4.699998f, -1.000004f},
            {-11.299999f, -4.699998f, -0.000004f},
            {-10.299999f, -4.699998f, -0.000004f},
            {-12.799999f, -4.699998f, -3.500004f},
            {-14.799999f, -4.699998f, -4.500004f},
            {-16.799999f, -4.699998f, -5.000004f},
            {1.700001f, -4.699998f, 19.499996f},
            {3.200001f, -5.199998f, 21.499996f},
            {-1.799999f, -5.199998f, 56.999996f},
            {0.200001f, -5.199998f, 56.999996f},
            {2.200001f, -5.199998f, 56.999996f},
            {-30.299999f, -5.199998f, 66.500000f},
            {-1.799999f, -5.199998f, 59.499996f},
            {-1.799999f, -5.199998f, 62.499996f},
            {-3.799999f, -5.199998f, 64.000000f},
            {-32.799999f, -5.199998f, 66.500000f},
            {-37.799999f, -5.199998f, 66.500000f},
            {-37.799999f, -5.199998f, 68.000000f},
            {-35.799999f, -5.199998f, 66.000000f},
            {-35.299999f, -5.199998f, 67.500000f},
            {-36.299999f, -5.199998f, 69.000000f},
            {-34.299999f, -5.199998f, 66.000000f},
            {-68.800003f, -5.199998f, 56.500000f},
            {-67.800003f, -5.199998f, 54.000000f},
            {-66.800003f, -4.699998f, 50.500000f},
            {-65.800003f, -4.699998f, 48.000000f},
            {-64.800003f, -4.699998f, 45.500000f},
            {-63.800003f, -4.699998f, 43.000000f},
            {-65.300003f, -4.199998f, 41.000000f},
            {-67.300003f, -4.199998f, 41.000000f},
            {-66.800003f, -4.199998f, 43.000000f},
            {-65.300003f, -4.199998f, 42.500000f},
            {-66.300003f, -4.199998f, 44.500000f},
            {-68.300003f, -4.199998f, 46.000000f},
            {-68.300003f, -4.199998f, 48.000000f},
            {-68.300003f, -4.199998f, 50.000000f},
            {-68.800003f, -4.199998f, 52.000000f},
            {-66.800003f, -4.199998f, 46.500000f},
            {50.199997f, -5.199998f, -159.500000f},
            {51.699997f, -5.199998f, -162.000000f},
            {53.199997f, -5.199998f, -170.000000f},
            {52.199997f, -5.199998f, -172.000000f},
            {52.199997f, -4.699998f, -167.500000f},
            {54.699997f, -4.699998f, -168.000000f},
            {56.699997f, -4.699998f, -169.000000f},
            {56.699997f, -4.699998f, -167.000000f},
            {55.699997f, -4.699998f, -165.500000f},
            {53.699997f, -4.699998f, -165.500000f},
            {54.699997f, -4.699998f, -163.500000f},
            {52.699997f, -4.699998f, -163.500000f},
            {51.699997f, -5.199998f, -166.500000f},
            {59.199997f, -4.699998f, -167.500000f},
            {61.199997f, -5.199998f, -170.000000f},
            {58.699997f, -5.199998f, -171.000000f},
            {56.199997f, -5.199998f, -171.000000f},
            {54.699997f, -5.199998f, -169.000000f},
            {51.199997f, -5.199998f, -169.000000f},
            {52.199997f, -5.199998f, -164.500000f},
            {52.199997f, -4.199998f, -162.000000f},
            {55.199997f, -4.199998f, -162.000000f},
            {58.699997f, -4.699998f, -165.000000f},
            {56.699997f, -4.199998f, -164.000000f},
            {62.699997f, -5.199998f, -168.000000f},
            {58.699997f, -4.699998f, -169.500000f},
            {58.199997f, -4.699998f, -169.000000f},
        };
        // Pine Tree Group 1
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * vmath::translate(8050.000000f, 1190.000000f, 38710.000000f) * scale(100.0f + 119.0f, 100.0f + 119.0f, 100.0f + 119.0f);
            model_tree.render_Models_Without_Texture(bezierPointsGroupSmall.size(), &(flatten(bezierPointsGroupSmall).at(0)));
        }
        modelMatrix = popMatrix();

        // Multiple Pine Tree.
        std::vector<std::vector<float>> bezierPointsMultiplePineTree = {
            {-6.200000f, -1.699998f, -19.299967f},
            {-12.200000f, -0.699998f, -22.299967f},
            {-16.700001f, 0.300002f, -23.299967f},
            {-17.700001f, -1.199998f, -17.299967f},
            {-10.700001f, -3.699998f, -12.799967f},
            {-15.700001f, -1.699998f, -17.799967f},
            {-22.200001f, -4.199998f, -9.799967f},
            {-20.200001f, -4.199998f, -11.299967f},
            {26.799999f, -3.699998f, -57.799965f},
            {34.299999f, -3.699998f, -52.299965f},
            {34.299999f, -4.699998f, -43.299965f},
            {25.799999f, -4.699998f, -48.799965f},
            {34.799999f, -3.199998f, -60.799965f},
        };
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * vmath::translate(8050.000000f, 1190.000000f, 38710.000000f) * scale(100.0f + 119.0f, 100.0f + 119.0f, 100.0f + 119.0f);
            model_treeGroup2.render_Models_Without_Texture(bezierPointsMultiplePineTree.size(), &(flatten(bezierPointsMultiplePineTree).at(0)));
        }
        modelMatrix = popMatrix();

        // Single Pine Tree

        std::vector<std::vector<float>> bezierPointsSinglePineTree = {
            {65.199997f, -5.199998f, -167.500000f},
            {66.699997f, -5.199998f, -168.000000f},
            {64.699997f, -5.199998f, -168.000000f},
            {64.199997f, -5.199998f, -168.500000f},
            {63.699997f, -5.199998f, -169.000000f},
            {66.199997f, -5.199998f, -167.500000f},
            {67.199997f, -5.199998f, -167.000000f},
            {68.699997f, -5.199998f, -166.500000f},
            {70.199997f, -5.199998f, -165.500000f},
            {69.699997f, -5.199998f, -166.000000f},
            {69.199997f, -5.199998f, -166.500000f},
            {68.199997f, -5.199998f, -167.000000f},
            {67.699997f, -5.199998f, -167.000000f},
            {70.699997f, -5.199998f, -166.000000f},
            {71.199997f, -5.199998f, -165.000000f},
            {70.699997f, -5.199998f, -165.000000f},
            {72.199997f, -5.199998f, -165.000000f},
            {72.199997f, -5.199998f, -166.000000f},
            {71.699997f, -5.199998f, -166.000000f},
            {73.699997f, -5.199998f, -166.500000f},
            {73.199997f, -5.199998f, -166.500000f},
            {72.199997f, -5.199998f, -166.500000f},
            {74.199997f, -5.199998f, -167.000000f},
            {74.199997f, -5.199998f, -168.000000f},
            {74.199997f, -5.199998f, -169.500000f},
            {74.699997f, -5.199998f, -170.000000f},
            {75.199997f, -5.199998f, -171.000000f},
            {75.699997f, -5.199998f, -170.000000f},
            {75.699997f, -5.199998f, -170.500000f},
            {74.699997f, -5.199998f, -168.000000f},
            {75.199997f, -5.199998f, -169.000000f},
            {74.699997f, -5.199998f, -170.500000f},
            {73.699997f, -5.199998f, -173.000000f},
            {73.199997f, -5.199998f, -173.500000f},
            {74.199997f, -5.199998f, -172.000000f},
            {74.699997f, -5.199998f, -168.500000f},
            {70.199997f, -5.299998f, -177.099976f},
            {71.099983f, -5.299998f, -177.099976f},
            {72.399963f, -5.299998f, -177.099976f},
            {72.499962f, -5.299998f, -175.599884f},
            {72.799957f, -5.299998f, -174.299805f},
            {72.499962f, -5.299998f, -174.899841f},
            {71.699974f, -5.399998f, -176.999969f},
            {70.599991f, -5.399998f, -176.999969f},
            {69.100014f, -5.399998f, -176.999969f},
            {69.400009f, -4.899998f, -176.999969f},
            {69.700005f, -4.899998f, -175.799896f},
            {68.400024f, -5.099998f, -175.799896f},
            {67.600037f, -5.099998f, -176.099915f},
            {66.800049f, -5.299998f, -176.499939f},
            {66.000061f, -5.299998f, -176.799957f},
            {65.500069f, -5.299998f, -177.500000f},
            {65.900063f, -5.299998f, -178.300049f},
            {65.300072f, -5.299998f, -178.300049f},
            {64.700081f, -5.299998f, -178.700073f},
            {64.500084f, -5.299998f, -179.300110f},
            {63.900093f, -5.299998f, -179.700134f},
            {63.900093f, -5.299998f, -180.300171f},
            {64.900078f, -4.999998f, -180.300171f},
            {65.200073f, -4.999998f, -179.800140f},
            {65.600067f, -5.199998f, -179.000092f},
            {65.100075f, -5.199998f, -179.000092f},
            {66.200058f, -5.199998f, -177.600006f},
            {66.800049f, -5.199998f, -177.500000f},
            {67.800034f, -5.299998f, -176.999969f},
            {67.100044f, -5.299998f, -176.999969f},
            {64.500084f, -5.299998f, -179.900146f},
            {63.400101f, -5.299998f, -180.400177f},
            {63.400101f, -5.299998f, -181.100220f},
            {63.900093f, -5.199998f, -180.800201f},
            {62.800110f, -5.499998f, -181.200226f},
            {63.200104f, -5.299998f, -181.700256f},
            {62.600113f, -5.299998f, -181.900269f},
            {62.100121f, -5.399998f, -182.100281f},
            {62.100121f, -5.399998f, -182.900330f},
            {61.600128f, -5.399998f, -183.200348f},
            {60.900139f, -5.399998f, -183.500366f},
            {61.500130f, -5.399998f, -182.500305f},
            {59.700157f, -5.399998f, -183.800385f},
            {59.100166f, -5.399998f, -183.800385f},
            {60.300148f, -5.399998f, -183.800385f},
            {58.100182f, -5.399998f, -183.400360f},
            {57.500191f, -5.399998f, -183.400360f},
            {57.500191f, -5.399998f, -184.000397f},
            {58.500175f, -5.399998f, -184.000397f},
            {58.100182f, -5.399998f, -184.300415f},
            {56.900200f, -5.399998f, -184.300415f},
            {56.900200f, -5.399998f, -183.600372f},
            {56.700203f, -5.399998f, -183.600372f},
        };
        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * vmath::translate(8050.000000f, 1190.000000f, 38710.000000f) * scale(100.0f + 119.0f, 100.0f + 119.0f, 100.0f + 119.0f);
            model_treeSingle.render_Models_Without_Texture(bezierPointsSinglePineTree.size(), &(flatten(bezierPointsSinglePineTree).at(0)));
        }
        modelMatrix = popMatrix();
    }

    void displayShadowObject(bool isDepthBuffer)
    {
        if (isDepthBuffer)
        {
            glBindVertexArray(shadowSphere->vao_sphere);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, shadowSphere->getNumberOfSphereVertices());
            glBindVertexArray(0);
        }
        else
        {
            glUseProgram(commonShaders->colorShader->shaderProgramObject);
            pushMatrix(modelMatrix);
            {
                glVertexAttrib4f(MATRIX_ATTRIBUTE_COLOR, 1.0, 1.0, 1.0, 1.0);
                //  translate(-5880.000000f, 7560.000000f, 4970.000000f) *
                modelMatrix = modelMatrix * vmath::rotate(-90.0f, 1.0f, 0.0f, 0.0f);
                /* Initialize uniforms constant throughout rendering loop. */
                glUniformMatrix4fv(commonShaders->colorShader->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
                glUniformMatrix4fv(commonShaders->colorShader->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
                glUniformMatrix4fv(commonShaders->colorShader->projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
                glBindVertexArray(shadowSphere->vao_sphere);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, shadowSphere->getNumberOfSphereVertices());
                glBindVertexArray(0);
            }
            modelMatrix = popMatrix();
            glUseProgram(0);
        }
    }
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

        // Camera
        if (sceneCamera.time < 1.0f)
            sceneCamera.time += (0.000015f + 0.0003f);
        // sceneCamera.time = globalTime;

        // sceneCamera.time = globalTime;
        terrain->updateTilesPositions();

        // // Trigger fadeout
        if (ELAPSED_TIME > (START_TIME_SCENE_04_TERRAIN_SHADOW_END - 2))
        {
            isFadeout = true;
        }

        // Shadow Translate
        if (ELAPSED_TIME > (START_TIME_SCENE_04_TERRAIN_SHADOW_START + 7) && shadowTranslate < maxShadowTranslate)
        {
            shadowTranslate += 0.07f;
        }

        terrain->setGrassCoverage(0.36f);
        terrain->setTextureTransitionFactor(0.8f);
        terrain->setWaterHeight(589.0f);
        waterMatrix->interpolateWaterColor = 1.13f;
        // terrain->setTextureTransitionFactor(1.0f);
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
