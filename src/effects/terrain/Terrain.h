#pragma once
#include "../../utils/common.h"
#include "../../utils/camera/Camera.h"
#include "../../shaders/terrain/TerrainShader.h"
#include "../water/Water.h"

extern Camera camera;

enum tPosition
{
    C,
    N,
    S,
    E,
    W,
    SE,
    SW,
    NE,
    NW,
    totTiles
};
float sign(float x)
{
    if (x > 0.0f)
        return 1.0f;
    else if (x < 0.0f)
    {
        return -1.0f;
    }
    else
        return 0.0f;
}

class Terrain
{
public:
    vmath::vec2 position, eps;
    float up = 0.0;
    int tileW = 10. * 100.;
    float waterHeight;
    vec3 terrainLightDirection;
    Terrain(int tileW_)
    {
        tileW = tileW_;
        int gl = 120;
        vmath::mat4 scaleMatrix = vmath::scale(1.0f, 0.0f, 1.0f);
        vmath::mat4 positionMatrix = vmath::translate(0.0f, 0.0f, 0.0f);
        modelMatrix = positionMatrix;

        octaves = 10;
        frequency = 0.01f;
        grassCoverage = 0.0f;
        tessMultiplier = 1.0f;
        dispFactor = 23.5f;

        fogFalloff = 0.5f;

        posBuffer = 0;

        // Calculating light direction
        terrainLightDirection = vmath::normalize(vec3(-0.4f, 0.4f, 0.816f));

        shad = new TerrainShader();
        if (!shad->initialize())
        {
            PrintLog("Terrain Shader Initialization Failed");
        }

        this->gridLength = gl + (gl + 1) % 2; // ensure gridLength is odd

        res = 4;
        initializePlaneVAO(res, tileW, &planeVAO, &planeVBO, &planeEBO);

        // Load Green Textures
        this->textures_green = new GLuint[6];
        if (LoadPNGImage(&textures_green[0], "./assets/textures/terrain/green/sand.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR sand");
        }
        if (LoadPNGImage(&textures_green[1], "./assets/textures/terrain/green/grass.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR grass");
        }
        if (LoadPNGImage(&textures_green[2], "./assets/textures/terrain/green/rdiffuse.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR rdiffuse");
        }
        if (LoadPNGImage(&textures_green[3], "./assets/textures/terrain/green/snow2.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR snow2");
        }
        if (LoadPNGImage(&textures_green[4], "./assets/textures/terrain/green/rnormal.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR rnormal");
        }
        if (LoadPNGImage(&textures_green[5], "./assets/textures/terrain/green/terrainTexture.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR terrainTexture");
        }

        // Load Dark Textures
        this->textures_dark = new GLuint[5];
        if (LoadPNGImage(&textures_dark[0], "./assets/textures/terrain/dark/sand.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR sand");
        }
        if (LoadPNGImage(&textures_dark[1], "./assets/textures/terrain/dark/grass.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR grass");
        }
        if (LoadPNGImage(&textures_dark[2], "./assets/textures/terrain/dark/rdiffuse.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR rdiffuse");
        }
        if (LoadPNGImage(&textures_dark[3], "./assets/textures/terrain/dark/snow2.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR snow2");
        }
        if (LoadPNGImage(&textures_dark[4], "./assets/textures/terrain/dark/terrainTexture.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR terrainTexture");
        }

        positionVec.resize(gridLength * gridLength);
        generateTileGrid(vmath::vec2(0.0, 0.0));

        setPositionsArray(positionVec);

        power = 3.0f;
    }

    // Shadow
    GLuint shadowMap;
    bool isShadow = false;
    vmath::vec3 shadowLightPosition;
    vmath::mat4 shadowLightSpaceMatrix;

    // Change light when shadow
    float shadowLightX, shadowLightY, shadowLightZ = 0.0f;

    void draw(float isShadow)
    {
        drawFog = true;

        if (up != 0.0f)
        {
            glEnable(GL_CLIP_DISTANCE0);
        }
        vmath::mat4 gWorld = modelMatrix;

        vmath::mat4 gVP = perspectiveProjectionMatrix * (USE_FPV_CAM ? camera.getViewMatrix() : globalBezierCamera->getViewMatrix());

        glUseProgram(shad->shaderProgramObject);
        shad->setVec3("u_gEyeWorldPos", (USE_FPV_CAM ? camera.getEye() : globalBezierCamera->getEye()));
        shad->setMat4("u_gWorld", gWorld);
        shad->setMat4("u_gVP", gVP);
        shad->setFloat("u_gDispFactor", dispFactor);

        // float waterHeight = (waterPtr ? waterPtr->getModelMatrix()[3][1] : 100.0);

        vmath::vec4 clipPlane(0.0f, 1.0f * up, 0.0f, -waterHeight * up);
        shad->setVec4("u_clipPlane", clipPlane);
        shad->setVec3("u_LightColor", vec3(255.0f, 255.0f, 230.0f) / 255.0f);

        terrainLightDirection = vmath::normalize(vec3(-0.4f + shadowLightX, 0.4f + shadowLightY, 0.816f + shadowLightZ));
        shad->setVec3("u_LightPosition", terrainLightDirection * 1e6f + (USE_FPV_CAM ? camera.getEye() : globalBezierCamera->getEye()));

        shad->setVec3("u_ViewPosition", (USE_FPV_CAM ? camera.getEye() : globalBezierCamera->getEye()));
        shad->setVec3("u_fogColor", vec3(0.1f, 0.1f, 0.2f));
        shad->setVec3("u_seed", vec3(12.0f, 20.0f, 10.0f));

        shad->setInt("u_octaves", octaves);
        shad->setFloat("u_freq", frequency);
        shad->setFloat("u_grassCoverage", grassCoverage);
        shad->setFloat("u_waterHeight", waterHeight);
        shad->setFloat("u_tessMultiplier", tessMultiplier);
        shad->setFloat("u_fogFalloff", fogFalloff * 1.e-6);
        shad->setFloat("u_power", power);
        shad->setFloat("u_transitionFactor", textureTransitionFactor);

        shad->setBool("u_normals", true);
        shad->setBool("u_drawFog", drawFog);

        // set textures
        // Green Textures
        shad->setSampler2D("sand_green", textures_green[0], 1);
        shad->setSampler2D("grass_green", textures_green[1], 2);
        shad->setSampler2D("rock_green", textures_green[2], 3);
        shad->setSampler2D("snow_green", textures_green[3], 4);
        shad->setSampler2D("grass1_green", textures_green[5], 5);

        // Normal Texture
        shad->setSampler2D("rockNormal", textures_green[4], 6);

        // Dark Textures
        shad->setSampler2D("sand_dark", textures_dark[0], 7);
        shad->setSampler2D("grass_dark", textures_dark[1], 8);
        shad->setSampler2D("rock_dark", textures_dark[2], 9);
        shad->setSampler2D("snow_dark", textures_dark[3], 10);
        shad->setSampler2D("grass1_dark", textures_dark[4], 11);

        // Set shadow variables
        if (isShadow)
        {
            shad->setBool("isShadow", isShadow);
            shad->setSampler2D("shadowMap", shadowMap, 12);
            shad->setVec3("shadowLightPosition", shadowLightPosition);
            shad->setMat4("u_lightSpaceMatrix", shadowLightSpaceMatrix);
        }

        int nIstances = positionVec.size();

        drawVertices(nIstances);

        glDisable(GL_CLIP_DISTANCE0);
        up = 0.0;
    }

    void updateTilesPositions()
    {
        int whichTile = -1;
        int howManyTiles = 0;

        vmath::vec2 currentTile;
        if (getWhichTileCameraIs(currentTile))
        {
            vmath::vec2 center = getPos(gridLength / 2, gridLength / 2);
            for (vmath::vec2 &p : positionVec)
            {
                p += currentTile - center;
            }
            setPositionsArray(positionVec);

            if (waterPtr)
            {
                vmath::vec2 center = getPos(gridLength / 2, gridLength / 2);
                waterPtr->setPosition(center, 1.0 * gridLength, waterPtr->getHeight());
            }
        }
    }

    void setPositionsArray(std::vector<vmath::vec2> &pos)
    {
        if (posBuffer)
        {
            this->deleteBuffer();
        }

        // vertex Buffer Object
        glGenBuffers(1, &posBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
        glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(vmath::vec2), &pos[0], GL_STATIC_DRAW);

        glBindVertexArray(planeVAO);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vmath::vec2), (void *)0);

        glVertexAttribDivisor(3, 1);
        glBindVertexArray(0);
    }

    bool inTile(Camera cam, vmath::vec2 pos)
    {
        float camX = cam.getEye()[0];
        float camY = cam.getEye()[2];

        float x = pos[0];
        float y = pos[1];

        bool inX = false;
        if ((camX <= x + 1.0f * tileW / 2.0f) && (camX >= x - 1.0f * tileW / 2.0f))
        {
            inX = true;
        }
        bool inY = false;
        if ((camY <= y + 1.0f * tileW / 2.0f) && (camY >= y - 1.0f * tileW / 2.0f))
        {
            inY = true;
        }

        return inX && inY;
    }

    bool inTile(BezierCamera *cam, vmath::vec2 pos)
    {
        float camX = cam->getEye()[0];
        float camY = cam->getEye()[2];

        float x = pos[0];
        float y = pos[1];

        bool inX = false;
        if ((camX <= x + 1.0f * tileW / 2.0f) && (camX >= x - 1.0f * tileW / 2.0f))
        {
            inX = true;
        }
        bool inY = false;
        if ((camY <= y + 1.0f * tileW / 2.0f) && (camY >= y - 1.0f * tileW / 2.0f))
        {
            inY = true;
        }

        return inX && inY;
    }

    // Model * planeModel;
    Water *waterPtr;

    void setOctaves(int oct)
    {
        if (oct > 0)
            octaves = oct;
    }

    void setFreq(float freq)
    {
        if (freq > 0.0f)
        {
            this->frequency = freq;
        }
    }

    void setDispFactor(float disp)
    {
        if (disp > 0.0f)
        {
            dispFactor = disp;
        }
    }

    void setScale(float scale)
    {
        vmath::mat4 scaleMatrix = vmath::scale(scale, 0.0f, scale);
        vmath::mat4 positionMatrix = vmath::translate(position[0] * scale / this->scaleFactor, 0.0f, position[1] * scale / this->scaleFactor);
        modelMatrix = positionMatrix * scaleMatrix;
        scaleFactor = scale;
    }

    void setWaterHeight(float wh)
    {
        waterHeight = wh;
    }
    void setGrassCoverage(float gc)
    {
        grassCoverage = gc;
    }
    void setTextureTransitionFactor(float ttf)
    {
        textureTransitionFactor = ttf;
    }

    void setTessMultiplier(float tm)
    {
        if (tm > 0.0)
            tessMultiplier = tm;
    }

    int getWaterHeight() const { return waterHeight; }
    int getOctaves() const { return octaves; }
    float getFreq() const { return frequency; }
    float getDispFactor() const { return dispFactor; }
    float getScale() const { return scaleFactor; }
    float getGrassCoverage() const { return grassCoverage; }
    float getTessMultiplier() const { return tessMultiplier; }
    float getTextureTransitionFactor() const { return textureTransitionFactor; }

private:
    void deleteBuffer()
    {
        glDeleteBuffers(1, &posBuffer);
        posBuffer = 0;
    }
    int res;
    GLuint planeVBO, planeVAO, planeEBO;
    float dispFactor, scaleFactor, frequency, grassCoverage, tessMultiplier, fogFalloff, power;
    int octaves;
    int gridLength;
    float textureTransitionFactor = -0.3f;
    bool drawFog;
    GLuint *textures_green, *textures_dark, posBuffer;

    TerrainShader *shad;
    vmath::mat4 modelMatrix;

    std::vector<vmath::vec2> positionVec;

    void drawVertices(int nInstances)
    {
        glBindVertexArray(planeVAO);
        glUseProgram(shad->shaderProgramObject);
        glDrawElementsInstanced(GL_PATCHES, (res - 1) * (res - 1) * 2 * 3, GL_UNSIGNED_INT, 0, nInstances);
        glBindVertexArray(0);
    }

    void setPos(int row, int col, vmath::vec2 pos)
    {
        positionVec[col + row * gridLength] = pos;
    }

    vmath::vec2 getPos(int row, int col)
    {
        return positionVec[col + row * gridLength];
    }

    void generateTileGrid(vmath::vec2 offset)
    {
        float sc = tileW;

        vmath::vec2 I = vmath::vec2(1, 0) * sc;
        vmath::vec2 J = vmath::vec2(0, 1) * sc;

        for (int i = 0; i < gridLength; i++)
        {
            for (int j = 0; j < gridLength; j++)
            {
                vmath::vec2 pos = (float)(j - gridLength / 2) * vmath::vec2(I) + (float)(i - gridLength / 2) * vmath::vec2(J);
                setPos(i, j, pos + offset);
            }
        }
    }

    bool getWhichTileCameraIs(vmath::vec2 &result)
    {
        // for (vmath::vec2 p : positionVec)
        // {
        //     if ((USE_FPV_CAM ? inTile(camera, p) : inTile(globalBezierCamera, p)))
        //     {
        //         result = p;
        //         return true;
        //     }
        // }
        return false;
    }

    void getColRow(int i, int &col, int &row)
    {
        col = (i) % gridLength;

        row = (i - col) / gridLength;
    }

    void uninitialize();
};
