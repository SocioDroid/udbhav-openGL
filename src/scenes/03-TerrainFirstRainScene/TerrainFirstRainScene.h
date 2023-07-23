#pragma once
#include "../../utils/common.h"
#include "../../effects/cubemap/Cubemap.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../shaders/smokeEarth/SmokeEarthShader.h"
#include "../../shaders/textureLight/TextureLightShader.h"
#include "../../shaders/terrainPostProcess/TerrainPostProcess.h"
#include "../../effects/terrain/Terrain.h"
#include "../../effects/water/Water.h"
#include "../../effects/water/buffers.h"

class TerrainFirstRainScene
{

public:
    bool isInitialized = false;
    Terrain *terrain;
    CubeMap *cubemap;
    Water *water;
    TerrainPostProcess postProcess;
    FrameBufferObject *SceneFBO;
    // member functions
    TerrainFirstRainScene()
    {
        // Terrain
        terrain = new Terrain();
        cubemap = new CubeMap();
        water = new Water(vmath::vec2(0.0, 0.0), 120, 120);
        SceneFBO = new FrameBufferObject(giWindowWidth, giWindowHeight);
    }

    BOOL initialize()
    {
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

        // Terrain Initialization
        terrain->waterPtr = water;

        // Post Processing
        postProcess.initialize();

        isInitialized = true;
        return TRUE;
    }

    void display()
    {
        pushMatrix(modelMatrix);
        {

            terrain->updateTilesPositions();
            water->setHeight(scaleX);
            // Scene
            SceneFBO->bind();
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

            glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            water->bindReflectionFBO();
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Invert camera
            if (USE_FPV_CAM)
            {
                camera.invertPitch();
                camera.position[1] -= 2 * (camera.position[1] - water->getHeight());
            }
            else
            {
                globalBezierCamera->invertPitch();
                globalBezierCamera->position[1] -= 2 * (globalBezierCamera->position[1] - water->getHeight());
            }

            terrain->up = 1.0;
            terrain->draw();
            FrameBufferObject const &reflFBO = water->getReflectionFBO();

            postProcess.disableTests();
            water->bindReflectionFBO();

            glUseProgram(postProcess.shaderProgramObject);
            postProcess.setVec2("resolution", vmath::vec2(1920, 1080));
            postProcess.setSampler2D("screenTexture", reflFBO.tex, 0);
            postProcess.setSampler2D("depthTex", reflFBO.depthTex, 2);
            // postProcess.setSampler2D("cloudTEX", reflectionVolumetricClouds.getCloudsRawTexture(), 1);
            postProcess.draw();

            postProcess.enableTests();
            if (USE_FPV_CAM)
            {
                camera.invertPitch();
                camera.position[1] += 2 * abs(camera.position[1] - water->getHeight());
            }
            else
            {
                globalBezierCamera->invertPitch();
                globalBezierCamera->position[1] += 2 * (globalBezierCamera->position[1] - water->getHeight());
            }

            water->bindRefractionFBO();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);

            terrain->up = -1.0;
            terrain->draw();

            // draw terrain and water
            SceneFBO->bind();
            terrain->draw();
            water->draw();

            // disable test for quad rendering
            postProcess.disableTests();

            // blend volumetric clouds rendering with terrain and apply some post process
            // /unbindCurrentFrameBuffer();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, giWindowWidth, giWindowHeight);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glUseProgram(postProcess.shaderProgramObject);
            postProcess.setVec2("resolution", vmath::vec2(giWindowWidth, giWindowHeight));
            if (USE_FPV_CAM)
            {
                postProcess.setVec3("cameraPosition", camera.getEye());
            }
            else
            {
                postProcess.setVec3("cameraPosition", globalBezierCamera->getEye());
            }

            postProcess.setSampler2D("screenTexture", SceneFBO->tex, 0);
            // postProcess.setSampler2D("cloudTEX", volumetricClouds.getCloudsTexture(), 1);
            postProcess.setSampler2D("depthTex", SceneFBO->depthTex, 2);
            // postProcess.setSampler2D("cloudDistance", volumetricClouds.getCloudsTexture(VolumetricClouds::cloudDistance), 3);

            // postProcess.setBool("wireframe", scene.wireframe);

            postProcess.setMat4("VP", perspectiveProjectionMatrix * camera.getViewMatrix());
            postProcess.draw();
        }
        modelMatrix = popMatrix();
    }

    void update()
    {
        // Transition to green texture
        if (terrain->getTextureTransitionFactor() < 1.0f)
        {
            terrain->setTextureTransitionFactor(terrain->getTextureTransitionFactor() + 0.001f);
        }
        // Grass Coverage
        if (terrain->getGrassCoverage() < 0.56f)
        {
            terrain->setGrassCoverage(terrain->getGrassCoverage() + 0.0002f);
        }
        terrain->setTextureTransitionFactor(1.0f);
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
