#pragma once

#include "../../utils/common.h"
#include "../../utils/camera/Camera.h"
#include "../../shaders/rain/RainShader.h"

class Rain
{
private:
public:
    Camera camera;
    RainShader *rainShader;

    GLuint vao;
    GLuint vbo_position;
    GLuint vbo_seed;
    GLuint vbo_velocity;
    // OpenCL Variables
    GLuint vbo_gpu_position;
    GLuint vbo_gpu_seed;
    GLuint vbo_gpu_velocity;

    GLuint projectionMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint modelMatrixUniform;

    int maxParticles;
    float posBuffer[280000] = {0};
    float seedBuffer[280000] = {0};
    float veloBuffer[280000] = {0};
    float clusterScale = 20.0f;
    float veloFactor = 250.0f;

    float dataBuffer[4 * 50000] = {0};
    vec3 eyePos = vec3(0.2f, 0.3f, 4.0f);
    vec3 winDir[512];
    float windForce = 10.0f;
    float dt = 0.0f;
    int windPtr = 0;
    // GLuint texture_rain[370];
    GLuint texture_rain;
    GLuint texture_rain_factor;

    // testing quad
    GLuint vao_quad;
    GLuint vbo_quad_position;
    GLuint vbo_quad_texcoord;

    // for light
    GLfloat lightAmbient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPosition[4] = {100.0f, 100.0f, 100.0f, 1.0f};
    GLfloat materialAmbient[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat materialDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat materialSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat materialShininess = 50.0f;

    GLfloat renderRain = true;

    float rainfactors[370] =
        {
            0.004535f, 0.014777f, 0.012512f, 0.130630f, 0.013893f, 0.125165f, 0.011809f, 0.244907f, 0.010722f, 0.218252f,
            0.011450f, 0.016406f, 0.015855f, 0.055476f, 0.015024f, 0.067772f, 0.021120f, 0.118653f, 0.018705f, 0.142495f,
            0.004249f, 0.017267f, 0.042737f, 0.036384f, 0.043433f, 0.039413f, 0.058746f, 0.038396f, 0.065664f, 0.054761f,
            0.002484f, 0.003707f, 0.004456f, 0.006006f, 0.004805f, 0.006021f, 0.004263f, 0.007299f, 0.004665f, 0.007037f,
            0.002403f, 0.004809f, 0.004978f, 0.005211f, 0.004855f, 0.004936f, 0.006266f, 0.007787f, 0.006973f, 0.007911f,
            0.004843f, 0.007565f, 0.007675f, 0.011109f, 0.007726f, 0.012165f, 0.013179f, 0.021546f, 0.013247f, 0.012964f,
            0.105644f, 0.126661f, 0.128746f, 0.101296f, 0.123779f, 0.106198f, 0.123470f, 0.129170f, 0.116610f, 0.137528f,
            0.302834f, 0.379777f, 0.392745f, 0.339152f, 0.395508f, 0.334227f, 0.374641f, 0.503066f, 0.387906f, 0.519618f,
            0.414521f, 0.521799f, 0.521648f, 0.498219f, 0.511921f, 0.490866f, 0.523137f, 0.713744f, 0.516829f, 0.743649f,
            0.009892f, 0.013868f, 0.034567f, 0.025788f, 0.034729f, 0.036399f, 0.030606f, 0.017303f, 0.051809f, 0.030852f,
            0.018874f, 0.027152f, 0.031625f, 0.023033f, 0.038150f, 0.024483f, 0.029034f, 0.021801f, 0.037730f, 0.016639f,
            0.002868f, 0.004127f, 0.133022f, 0.013847f, 0.123368f, 0.012993f, 0.122183f, 0.015031f, 0.126043f, 0.015916f,
            0.002030f, 0.002807f, 0.065443f, 0.002752f, 0.069440f, 0.002810f, 0.081357f, 0.002721f, 0.076409f, 0.002990f,
            0.002425f, 0.003250f, 0.003180f, 0.011331f, 0.002957f, 0.011551f, 0.003387f, 0.006086f, 0.002928f, 0.005548f,
            0.003664f, 0.004258f, 0.004269f, 0.009404f, 0.003925f, 0.009233f, 0.004224f, 0.009405f, 0.004014f, 0.008435f,
            0.038058f, 0.040362f, 0.035946f, 0.072104f, 0.038315f, 0.078789f, 0.037069f, 0.077795f, 0.042554f, 0.073945f,
            0.124160f, 0.122589f, 0.121798f, 0.201886f, 0.122283f, 0.214549f, 0.118196f, 0.192104f, 0.122268f, 0.209397f,
            0.185212f, 0.181729f, 0.194527f, 0.420721f, 0.191558f, 0.437096f, 0.199995f, 0.373842f, 0.192217f, 0.386263f,
            0.003520f, 0.053502f, 0.060764f, 0.035197f, 0.055078f, 0.036764f, 0.048231f, 0.052671f, 0.050826f, 0.044863f,
            0.002254f, 0.023290f, 0.082858f, 0.043008f, 0.073780f, 0.035838f, 0.080650f, 0.071433f, 0.073493f, 0.026725f,
            0.002181f, 0.002203f, 0.112864f, 0.060140f, 0.115635f, 0.065531f, 0.093277f, 0.094123f, 0.093125f, 0.144290f,
            0.002397f, 0.002369f, 0.043241f, 0.002518f, 0.040455f, 0.002656f, 0.002540f, 0.090915f, 0.002443f, 0.101604f,
            0.002598f, 0.002547f, 0.002748f, 0.002939f, 0.002599f, 0.003395f, 0.002733f, 0.003774f, 0.002659f, 0.004583f,
            0.003277f, 0.003176f, 0.003265f, 0.004301f, 0.003160f, 0.004517f, 0.003833f, 0.008354f, 0.003140f, 0.009214f,
            0.008558f, 0.007646f, 0.007622f, 0.026437f, 0.007633f, 0.021560f, 0.007622f, 0.017570f, 0.007632f, 0.018037f,
            0.031062f, 0.028428f, 0.028428f, 0.108300f, 0.028751f, 0.111013f, 0.028428f, 0.048661f, 0.028699f, 0.061490f,
            0.051063f, 0.047597f, 0.048824f, 0.129541f, 0.045247f, 0.124975f, 0.047804f, 0.128904f, 0.045053f, 0.119087f,
            0.002197f, 0.002552f, 0.002098f, 0.200688f, 0.002073f, 0.102060f, 0.002111f, 0.163116f, 0.002125f, 0.165419f,
            0.002060f, 0.002504f, 0.002105f, 0.166820f, 0.002117f, 0.144274f, 0.005074f, 0.143881f, 0.004875f, 0.205333f,
            0.001852f, 0.002184f, 0.002167f, 0.163804f, 0.002132f, 0.212644f, 0.003431f, 0.244546f, 0.004205f, 0.315848f,
            0.002450f, 0.002360f, 0.002243f, 0.154635f, 0.002246f, 0.148259f, 0.002239f, 0.348694f, 0.002265f, 0.368426f,
            0.002321f, 0.002393f, 0.002376f, 0.074124f, 0.002439f, 0.126918f, 0.002453f, 0.439270f, 0.002416f, 0.489812f,
            0.002484f, 0.002629f, 0.002559f, 0.150246f, 0.002579f, 0.140103f, 0.002548f, 0.493103f, 0.002637f, 0.509481f,
            0.002960f, 0.002952f, 0.002880f, 0.294884f, 0.002758f, 0.332805f, 0.002727f, 0.455842f, 0.002816f, 0.431807f,
            0.003099f, 0.003028f, 0.002927f, 0.387154f, 0.002899f, 0.397946f, 0.002957f, 0.261333f, 0.002909f, 0.148548f,
            0.004887f, 0.004884f, 0.006581f, 0.414647f, 0.003735f, 0.431317f, 0.006426f, 0.148997f, 0.003736f, 0.080715f,
            0.001969f, 0.002159f, 0.002325f, 0.200211f, 0.002288f, 0.202137f, 0.002289f, 0.595331f, 0.002311f, 0.636097f};

    Rain(int particleSize)
    {
        maxParticles = particleSize;
    }

    BOOL initialize(GLuint textureNumber)
    {
        // OpenCL Initialization

        GLfloat position[] = {
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f};

        GLfloat coord[] = {
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f};

        glGenVertexArrays(1, &vao_quad);
        glBindVertexArray(vao_quad);
        {
            glGenBuffers(1, &vbo_quad_position);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_quad_position);
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
                glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, false, 0, 0);
                glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glGenBuffers(1, &vbo_quad_texcoord);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_quad_texcoord);
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(coord), coord, GL_STATIC_DRAW);
                glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, false, 0, 0);
                glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        glBindVertexArray(0);

        // initializing Rain Shader
        rainShader = new RainShader();
        rainShader->initializeRainShader();
        PrintLog("rain shader initialized\n");

        // VAO and VBO Related Code

        // VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // // VBO For CPU
        glGenBuffers(1, &vbo_position);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
        glBufferData(GL_ARRAY_BUFFER, 4 * maxParticles * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &vbo_seed);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_seed);
        glBufferData(GL_ARRAY_BUFFER, 4 * maxParticles * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_SEED, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_SEED);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &vbo_velocity);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_velocity);
        glBufferData(GL_ARRAY_BUFFER, 4 * maxParticles * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_VELOCITY, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_VELOCITY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Unbinding of VAO
        glBindVertexArray(0);
        // Create OpenCL-OpenGL Interoperability Resource

        // loading textures for rain
        if (Load1DPNGImage(&texture_rain_factor, rainfactors) == FALSE)
        {
            PrintLog("Failed to load Rain rainfactors\n");
            return FALSE;
        }

        if (textureNumber == 1)
        {
            if (LoadPNGImage(&texture_rain, "./assets/textures/Rain/rain-light.png") == FALSE)
            {
                PrintLog("Failed to load Rain texture\n");
                return FALSE;
            }
        }
        else if (textureNumber == 2)
        {
            if (LoadPNGImage(&texture_rain, "./assets/textures/Rain/rain-heavy.png") == FALSE)
            {
                PrintLog("Failed to load Rain texture\n");
                return FALSE;
            }
        }

        return TRUE;
    }

    void createRainData(void)
    {
        // code
        posBuffer[4 * maxParticles * sizeof(float)];
        seedBuffer[4 * maxParticles * sizeof(float)];
        veloBuffer[4 * maxParticles * sizeof(float)];

        dataBuffer[sizeof(posBuffer) * 3];

        int numLodLv1 = 8;
        eyePos = (USE_FPV_CAM ? camera.getEye() : globalBezierCamera->getEye());
        // eyePos = vec3(1.0f, 1.0f, 1.0f);

        for (int lodLv1 = 0; lodLv1 < numLodLv1; lodLv1++)
        {
            for (int i = 0; i < (maxParticles / numLodLv1); i++)
            {
                // spawning position
                float x, y, z;
                do
                {
                    x = (randFloat(0.0f, 1.0f) - 0.5f) * (clusterScale + lodLv1 * 0.3);
                    // x = (randomf() - 0.5f) * (clusterScale +  5);
                    y = (randFloat(0.0f, 1.0f) + 0.1f) * (clusterScale * 0.8);
                    z = (randFloat(0.0f, 1.0f) - 0.5f) * (clusterScale + lodLv1 * 5);
                    // z = (randomf() - 0.5f) * (clusterScale +  5);
                } while ((z < 0.2f && z > -0.2f) && (x < 0.2f && x > -0.2f));
                // respawn if particle is too close to viewer

                // add to position buffer
                posBuffer[i] = x + eyePos[0];
                posBuffer[i + 1] = y + eyePos[1];
                posBuffer[i + 2] = z + eyePos[2];
                posBuffer[i + 3] = 1.0f;

                // add to seed buffer
                // add random type to w coordinate in buffer
                // type is for choosing 1 out of 8 different textures
                seedBuffer[i] = x;
                seedBuffer[i + 1] = y;
                seedBuffer[i + 2] = z;
                seedBuffer[i + 3] = (float)randInt(1, 8);

                // add spawning velocity (small random velocity in x- and z-direction for variety and AA
                veloBuffer[i] = veloFactor * (randFloat(0.0f, 1.0f) / 100.0f);
                veloBuffer[i + 1] = veloFactor * ((randFloat(0.0f, 1.0f) + 1.0f) / 20.0f);
                veloBuffer[i + 2] = veloFactor * (randFloat(0.0f, 1.0f) / 100.0f);
                // add random number in w coordinate, used to light up random streaks
                //  float tmpR = randomf();
                float tmpR = randFloat(0.0f, 1.0f);
                if (tmpR > 0.8f)
                {
                    // *(veloBuffer+3) = 1.0f + (1.0f - tmpR); // 1.0 to 1.2
                    veloBuffer[i + 3] = 1.0f + (1.0f - tmpR);
                }
                else
                {
                    veloBuffer[i + 3] = 1.0f;
                }
            }
        }
    }

    void createWindData(void)
    {
        // code
        float windRand = randFloat(0.0f, 1.0f) * windForce;
        // use sine to create impression of wind gust
        for (int i = 0; i < (512); i++)
        {
            winDir[i][0] = (float)(sin((float)i / (float)sizeof(winDir)) * windRand);

            winDir[i][1] = 0.0f;

            winDir[i][2] = (float)(sin((float)i / (float)sizeof(winDir)) * windRand);
        }
    }

    void display(void)
    {
        pushMatrix(modelMatrix);
        {
            display_rain();
        }
        modelMatrix = popMatrix();
    }

    void display_rain(void)
    {
        // Variable Declarations
        createRainData();

        glUseProgram(rainShader->shaderProgramObject);

        glUniformMatrix4fv(rainShader->modelMatrixUniform, 1, GL_FALSE, vmath::translate(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(rainShader->viewMatrixUniform, 1, GL_FALSE, mat4::identity());
        glUniformMatrix4fv(rainShader->projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

        glUniform3fv(rainShader->eyePosUniform, 1, (USE_FPV_CAM ? camera.getEye() : globalBezierCamera->getEye()));
        glUniform3fv(rainShader->windDirUniform, 1, winDir[windPtr]);
        glUniform1f(rainShader->dtUniform, dt);

        // FOR LIGHT
        glUniform3fv(rainShader->laUniform, 1, lightAmbient);
        glUniform3fv(rainShader->ldUniform, 1, lightDiffuse);
        glUniform3fv(rainShader->lsUniform, 1, lightSpecular);
        glUniform4fv(rainShader->lightPositionUniform, 1, lightPosition);

        glUniform3fv(rainShader->kaUniform, 1, materialAmbient);
        glUniform3fv(rainShader->kdUniform, 1, materialDiffuse);
        glUniform3fv(rainShader->ksUniform, 1, materialSpecular);
        glUniform1f(rainShader->materialShinessUniform, materialShininess);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_rain);
        glUniform1i(rainShader->rainTextureSamplerUniform, 0);

        glEnable(GL_TEXTURE_1D);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_1D, texture_rain_factor);
        glUniform1i(rainShader->rainfactorsSamplerUniform, 1);

        glUniform3fv(rainShader->sunDirUniform, 1, vec3(0.0f, -1.0f, 0.0f));
        glUniform3fv(rainShader->sunColorUniform, 1, vec3(252.0f / 255.0f, 229.0f / 255.0f, 112.0f / 255.0f));
        glUniform1f(rainShader->sunIntensityUniform, 5.0f);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
        glBufferData(GL_ARRAY_BUFFER, 4 * maxParticles * sizeof(float), posBuffer, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_seed);
        glBufferData(GL_ARRAY_BUFFER, 4 * maxParticles * sizeof(float), seedBuffer, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_SEED, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_SEED);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_velocity);
        glBufferData(GL_ARRAY_BUFFER, 4 * maxParticles * sizeof(float), veloBuffer, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_VELOCITY, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_VELOCITY);

        glDrawArrays(GL_POINTS, 0, maxParticles);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_1D, 0);
        glDisable(GL_TEXTURE_1D);

        glBindVertexArray(0);

        glUseProgram(0);

        dt = 1e-3f * ELAPSED_TIME;
    }

    void display1(void)
    {
        glUseProgram(commonShaders->textureShader->shaderProgramObject);

        glUniformMatrix4fv(commonShaders->textureShader->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(commonShaders->textureShader->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(commonShaders->textureShader->projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

        glEnable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_rain);
        glUniform1i(commonShaders->textureShader->textureSamplerUniform, 0);

        glBindVertexArray(vao_quad);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);

        glUseProgram(0);
    }

    float randomf()
    {
        return (float((double)rand() / ((double)(RAND_MAX) + (double)(1))));
    }

    float randFloat(float min, float max)
    {
        return min + (rand() / (RAND_MAX / (max - min)));
    }

    float randInt(int min, int max)
    {
        return min + (rand() / (RAND_MAX / (max - min)));
    }

    void uninitialize(void)
    {
        if (rainShader)
        {
            rainShader->uninitialize();
            delete rainShader;
            rainShader = NULL;
        }

        if (texture_rain)
        {
            glDeleteTextures(1, &texture_rain);
            texture_rain = 0;
        }

        if (texture_rain_factor)
        {
            glDeleteTextures(1, &texture_rain_factor);
            texture_rain_factor = 0;
        }

        if (vbo_quad_texcoord)
        {
            glDeleteBuffers(1, &vbo_quad_texcoord);
            vbo_quad_texcoord = 0;
        }

        if (vbo_quad_position)
        {
            glDeleteBuffers(1, &vbo_quad_position);
            vbo_quad_position = 0;
        }

        if (vao_quad)
        {
            glDeleteVertexArrays(1, &vao_quad);
            vao_quad = 0;
        }
    }
};
