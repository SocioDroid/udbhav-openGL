#pragma once
#include "../../utils/common.h"
#include "../../effects/cubemap/Cubemap.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../shaders/bloom/HDR_Shader.h"
#include "../../shaders/smokeEarth/SmokeEarthShader.h"
#include "../../effects/bloom/Bloom_Shaders.h"
#include "../../shaders/textureLight/TextureLightShader.h"
#include "../../effects/terrain/Terrain.h"

class TerrainFirstRainScene
{

public:
    bool isInitialized = false;
    Terrain *terrain;
    CubeMap *cubemap;

    // Bloom
    BloomShaders bloomShader;
    GLuint fbo_UserMap; // FRAME BUFFER OBJECT
    GLuint rbo_UserMap; // RENDER BUFFER OBJECT
    GLuint fbo_texture_UserMap;

    // member functions
    TerrainFirstRainScene()
    {
        // Terrain
        terrain = new Terrain();
        cubemap = new CubeMap();
    }

    BOOL initialize()
    {
        // Bloom
        bloomShader.initialize_bloomShaderObject();
        bloomShader.exposure = 0.2f;
        if (bloomShader.CreateSceneFBO(1920, 1080, fbo_UserMap, rbo_UserMap, fbo_texture_UserMap) == false)
        {
            PrintLog("Failed to create Scene FBO\n");
            return FALSE;
        }

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

        isInitialized = true;
        return TRUE;
    }

    void display()
    {
        // Bloom
        // pushMatrix(modelMatrix);
        // {
        //     glBindFramebuffer(GL_FRAMEBUFFER, fbo_UserMap);
        //     {
        //         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //         glViewport(0, 0, 1920, 1080);
        //         displayScene();
        //     }
        //     glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // }
        // modelMatrix = popMatrix();

        // pushMatrix(modelMatrix);
        // {

        //     bloomShader.bindBloomFBO();
        //     {
        //         // Applying bloom only to spheres
        //         drawBloomObjects();
        //     }
        //     bloomShader.unbindBloomFBO();

        //     bloomShader.renderBlurFBO();
        //     bloomShader.renderFinalBloomScene(fbo_texture_UserMap);
        // }
        // modelMatrix = popMatrix();

        displayScene();
    }

    void displayScene()
    {
        // Cubemap
        // pushMatrix(modelMatrix);
        // {
        //     modelMatrix = modelMatrix * vmath::scale(1000.0f, 1000.0f, 1000.0f);
        //     cubemap->display();
        // }
        // modelMatrix = popMatrix();

        pushMatrix(modelMatrix);
        {
            drawBloomObjects();
        }
        modelMatrix = popMatrix();

        // Terrain
        pushMatrix(modelMatrix);
        {
            terrain->updateTilesPositions();
            terrain->up = 1.0;
            terrain->draw();
        }
        modelMatrix = popMatrix();
    }

    void drawBloomObjects()
    {
        // Draw objects that need to have bloom
    }
    void update()
    {
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
