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

        pushMatrix(modelMatrix);
        {
            modelMatrix = modelMatrix * vmath::translate(8050.000000f, 1190.000000f, 38710.000000f) * scale(100.0f + scaleX, 100.0f + scaleX, 100.0f + scaleX);
            model_tree.render_Models_Without_Texture(bezierPoints.size(), &(flatten(bezierPoints).at(0)));
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

        // // // Trigger fadeout
        // if (ELAPSED_TIME > (START_TIME_SCENE_04_TERRAIN_SHADOW_END - 2))
        // {
        //     isFadeout = true;
        // }

        // // Shadow Translate
        // if (ELAPSED_TIME > (START_TIME_SCENE_04_TERRAIN_SHADOW_START - 3) && shadowTranslate < maxShadowTranslate)
        // {
        //     shadowTranslate += 0.07f;
        // }

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
