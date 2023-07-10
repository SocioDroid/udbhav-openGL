#pragma once
#include "color/ColorShader.h"
#include "texture/TextureShader.h"
#include "textureDisintegrate/TextureDisintegrateShader.h"
#include "model/Model_ShaderCommon.h"
#include "model/Model_ShaderParticleCommon.h"
#include "overlayTexture/OverlayTextureShader.h"
#include "overlayColor/OverlayColorShader.h"

class CommonShaders
{
public:
    // Variables
    ColorShader *colorShader;
    TextureShader *textureShader;
    TextureDisintegrateShader *textureDisintegrateShader;
    ModelShaderCommon *modelShaderCommon;
    OverlayTextureShader *overlayTextureShader;
    OverlayColorShader *overlayColorShader;
    Model_ShaderParticleCommon *particleShader;

    // Member Functions
    BOOL initialize()
    {
        colorShader = new ColorShader();
        textureShader = new TextureShader();
        textureDisintegrateShader = new TextureDisintegrateShader();
        modelShaderCommon = new ModelShaderCommon();
        overlayTextureShader = new OverlayTextureShader();
        overlayColorShader = new OverlayColorShader();
        particleShader = new Model_ShaderParticleCommon();

        if (!colorShader->initialize())
        {
            PrintLog("Error in initializing colorShader");
        }

        if (!textureShader->initialize())
        {
            PrintLog("Error in initializing textureShader");
        }

        if (!textureDisintegrateShader->initialize())
        {
            PrintLog("Error in initializing textureDisintegrateShader");
        }

        if (!modelShaderCommon->initialize())
        {
            PrintLog("Error in initializing modelShaderCommon");
        }

        if (!overlayTextureShader->initialize())
        {
            PrintLog("Error in initializing overlayTextureShader");
        }

        if (!overlayColorShader->initialize())
        {
            PrintLog("Error in initializing overlayColorShader");
        }

        if (!particleShader->initialize())
        {
            PrintLog("Error in initializing particleShader");
        }

        return TRUE;
    }

    void uninitialize(void)
    {
        if (colorShader)
        {
            colorShader->uninitialize();
            free(colorShader);
        }

        if (textureShader)
        {
            textureShader->uninitialize();
            free(textureShader);
        }

        if (textureDisintegrateShader)
        {
            textureDisintegrateShader->uninitialize();
            free(textureDisintegrateShader);
        }

        if (modelShaderCommon)
        {
            modelShaderCommon->uninitialize_ModelShaderObject();
            free(modelShaderCommon);
        }
        if (overlayTextureShader)
        {
            overlayTextureShader->uninitialize();
            free(overlayTextureShader);
        }
        if (overlayColorShader)
        {
            overlayColorShader->uninitialize();
            free(overlayColorShader);
        }
        if (particleShader)
        {
            particleShader->uninitialize();
            free(particleShader);
        }
    }
};
