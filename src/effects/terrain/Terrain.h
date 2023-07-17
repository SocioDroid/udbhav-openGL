#pragma once
#include "../../utils/common.h"
#include "../../utils/camera/Camera.h"
#include "../../shaders/terrain/TerrainShader.h"

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
    Terrain()
    {
        int gl = 120;
        vmath::mat4 scaleMatrix = vmath::scale(1.0f, 0.0f, 1.0f);
        vmath::mat4 positionMatrix = vmath::translate(0.0f, 0.0f, 0.0f);
        modelMatrix = positionMatrix;

        octaves = 13;
        frequency = 0.01f;
        grassCoverage = 0.65f;
        tessMultiplier = 1.0f;
        dispFactor = 20.0f;

        fogFalloff = 1.5f;

        posBuffer = 0;

        shad = new TerrainShader();
        if (!shad->initialize())
        {
            PrintLog("Terrain Shader Initialization Failed");
        }

        this->gridLength = gl + (gl + 1) % 2; // ensure gridLength is odd

        res = 4;
        initializePlaneVAO(res, tileW, &planeVAO, &planeVBO, &planeEBO);

        // load a bunch of textures
        this->textures = new GLuint[6];
        if (LoadPNGImage(&textures[0], "./assets/textures/terrain/sand.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR sand");
        }
        if (LoadPNGImage(&textures[1], "./assets/textures/terrain/grass.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR grass");
        }
        if (LoadPNGImage(&textures[2], "./assets/textures/terrain/rdiffuse.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR rdiffuse");
        }
        if (LoadPNGImage(&textures[3], "./assets/textures/terrain/snow2.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR snow2");
        }
        if (LoadPNGImage(&textures[4], "./assets/textures/terrain/rnormal.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR rnormal");
        }
        if (LoadPNGImage(&textures[5], "./assets/textures/terrain/terrainTexture.png") == FALSE)
        {
            PrintLog("\t\t IMAGE LOADING FAILED FOR terrainTexture");
        }

        positionVec.resize(gridLength * gridLength);
        generateTileGrid(vmath::vec2(0.0, 0.0));

        setPositionsArray(positionVec);

        rockColor = vmath::vec3(255.0f * 1.5f, 255.0f * 1.5f, 255.0f * 1.5f) / 255.f;
        power = 3.0f;
    }

    void draw()
    {
        drawFog = false;

        if (up != 0.0f)
        {
            glEnable(GL_CLIP_DISTANCE0);
        }
        vmath::mat4 gWorld = modelMatrix;
        vmath::mat4 gVP = perspectiveProjectionMatrix * viewMatrix;

        glUseProgram(shad->shaderProgramObject);
        shad->setVec3("gEyeWorldPos", camera.getEye() * 5.0f);
        shad->setMat4("gWorld", gWorld);
        shad->setMat4("gVP", gVP);
        shad->setFloat("gDispFactor", dispFactor);

        // float waterHeight = (waterPtr ? waterPtr->getModelMatrix()[3][1] : 100.0);
        float waterHeight = 100.0f;
        vmath::vec4 clipPlane(0.0f, 1.0f * up, 0.0f, -waterHeight * up);
        shad->setVec4("clipPlane", clipPlane);
        shad->setVec3("u_LightColor", vec3(255.0f, 255.0f, 230.0f) / 255.0f);

        // Calculating light direction
        vec3 terrainLightDirection = vmath::normalize(vec3(0.0f, 10.0f, objY));

        shad->setVec3("u_LightPosition", terrainLightDirection * 1e6f + camera.getEye());
        shad->setVec3("u_ViewPosition", camera.getEye());
        shad->setVec3("fogColor", vec3(0.5f, 0.6f, 0.7f));
        shad->setVec3("rockColor", rockColor);
        shad->setVec3("seed", vec3(0.0f, 0.0f, 0.0f));

        shad->setInt("octaves", octaves);
        shad->setFloat("freq", frequency);
        shad->setFloat("u_grassCoverage", grassCoverage);
        shad->setFloat("waterHeight", waterHeight);
        shad->setFloat("tessMultiplier", tessMultiplier);
        shad->setFloat("fogFalloff", fogFalloff * 1.e-6);
        shad->setFloat("power", power);

        shad->setBool("normals", true);
        shad->setBool("drawFog", drawFog);

        // set textures
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        shad->setInt("sand", 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        shad->setInt("grass", 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, textures[2]);
        shad->setInt("rock", 3);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, textures[3]);
        shad->setInt("snow", 4);

        shad->setSampler2D("grass1", textures[5], 5);

        shad->setSampler2D("rockNormal", textures[4], 6);

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

            // if (waterPtr)
            // {
            //     vmath::vec2 center = getPos(gridLength / 2, gridLength / 2);
            //     waterPtr->setPosition(center, 1.0 * gridLength, waterPtr->getHeight());
            // }
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

    vmath::vec2 position, eps;
    float up = 0.0;

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
    static const int tileW = 10. * 100.;

    // Model * planeModel;
    //  Water * waterPtr;

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

    void setGrassCoverage(float gc)
    {
        grassCoverage = gc;
    }

    void setTessMultiplier(float tm)
    {
        if (tm > 0.0)
            tessMultiplier = tm;
    }

    int getOctaves() const { return octaves; }
    float getFreq() const { return frequency; }
    float getDispFactor() const { return dispFactor; }
    float getScale() const { return scaleFactor; }
    float getGrassCoverage() const { return grassCoverage; }
    float getTessMultiplier() const { return tessMultiplier; }

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
    vmath::vec3 rockColor;

    bool drawFog;
    GLuint *textures, posBuffer;

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
        for (vmath::vec2 p : positionVec)
        {
            if (inTile(camera, p))
            {
                result = p;
                return true;
            }
        }
        return false;
    }

    void getColRow(int i, int &col, int &row)
    {
        col = (i) % gridLength;

        row = (i - col) / gridLength;
    }

    void reset();
};
