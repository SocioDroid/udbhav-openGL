#pragma once
#include "../../utils/common.h"
#include "../../effects/metaballARM/Metaball.h"
#include "../../effects/cubemap/Cubemap.h"
#include "../../utils/camera/BezierCamera.h"
#include "../../effects/starfield/starfield.h"
#include "../../shaders/bloom/HDR_Shader.h"
#include "../../effects/bloom/Bloom_Shaders.h"

void setGlobalBezierCamera(BezierCamera *bezierCamera);

class EarthBirthScene
{

public:
    // member variables
    int selected_scene;
    bool START_E2E_DEMO;

    // Scenes
    Metaball *metaball;
    StarField *starfield;
    CubeMap *cubemap;

    // Bloom
    BloomShaders bloomShader;
    GLuint fbo_UserMap; // FRAME BUFFER OBJECT
    GLuint rbo_UserMap; // RENDER BUFFER OBJECT
    GLuint fbo_texture_UserMap;

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

        return TRUE;
    }

    void display()
    {
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
                    modelMatrix = modelMatrix * vmath::translate(0.0f, 0.0f, 8.0f);
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

    void displayScene()
    {
        // Cubemap
        pushMatrix(modelMatrix);
        {
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
    }
    void update()
    {
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
