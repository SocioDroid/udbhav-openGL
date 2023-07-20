#pragma once

#include "../../utils/common.h"
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLLog.h"
#include "../../shaders/starfield/starFieldShader.h"

class StarField
{
public:
    StarFieldShader *starfieldShader = NULL;

    // Random number generator
    unsigned int seed = 0x13371337;

    struct star_t
    {
        GLfloat position[3];
    };
    star_t *starPos = NULL;

    struct Star_color
    {
        GLfloat color[3];
    };
    Star_color *starColor = NULL;

    int NUM_STARS = 500;

    GLuint vao;
    GLuint vbo_position;
    GLuint vbo_color;

    GLuint starFieldTexture;

    float timeUniform;
    double timerStart;

    int initialize()
    {
        starfieldShader = new StarFieldShader();

        starfieldShader->initialize();
        initVertices();
        initColor();

        // VAO related code
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // VBO For Postition
        glGenBuffers(1, &vbo_position);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
        glBufferData(GL_ARRAY_BUFFER, sizeof(star_t) * NUM_STARS, starPos, GL_STATIC_DRAW);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // VBO For Color
        glGenBuffers(1, &vbo_color);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Star_color) * NUM_STARS, starColor, GL_STATIC_DRAW);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_COLOR);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        if (LoadPNGImage(&starFieldTexture, "./assets/textures/starfield/star.png") == FALSE)
        {
            PrintLog("LoadGLTexture Failed for texture_starField..\n");
            return (-6);
        }

        return (TRUE);
    }

    void display(void)
    {
        // code
        glUseProgram(starfieldShader->shaderProgramObject);
        {
            glEnable(GL_PROGRAM_POINT_SIZE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glUniformMatrix4fv(starfieldShader->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
            glUniformMatrix4fv(starfieldShader->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
            glUniformMatrix4fv(starfieldShader->projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
            glUniform1f(starfieldShader->timeUniform, ELAPSED_TIME * 0.005f);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, starFieldTexture);

            glEnable(GL_POINT_SPRITE);
            glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
            glUniform1i(starfieldShader->textureSamplerUniform, 0);

            glBindVertexArray(vao);
            glDrawArrays(GL_POINTS, 0, NUM_STARS);
            glBindTexture(GL_TEXTURE_2D, 0);

            glDisable(GL_POINT_SPRITE);
            glDisable(GL_PROGRAM_POINT_SIZE);
            glDisable(GL_BLEND);
            glBindVertexArray(0);
        }
        glUseProgram(0);
    }

    float random_float()
    {
        float res;
        unsigned int tmp;

        seed *= 16807;

        tmp = seed ^ (seed >> 4) ^ (seed << 15);

        *((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;

        return (res - 1.0f);
    }

    void initVertices(void)
    {

        starPos = (star_t *)malloc(sizeof(star_t) * NUM_STARS);
        for (int i = 0; i < NUM_STARS; i++)
        {
            starPos[i].position[0] = (random_float() * 5.0f - 1.0f) * 100.0f;
            starPos[i].position[1] = (random_float() * 5.0f - 1.0f) * 100.0f;
            starPos[i].position[2] = random_float();
        }
    }

    void initColor(void)
    {
        starColor = (Star_color *)malloc(sizeof(Star_color) * NUM_STARS);
        for (int i = 0; i < NUM_STARS; i++)
        {
            starColor[i].color[0] = 0.8f + random_float() * 0.2f;
            starColor[i].color[1] = 0.8f + random_float() * 0.2f;
            starColor[i].color[2] = 0.8f + random_float() * 0.2f;
        }
    }

    void uninitialize(void)
    {
        starfieldShader->uninitialize();
        if (starFieldTexture)
        {
            glDeleteTextures(1, &starFieldTexture);
            starFieldTexture = 0;
        }

        if (vbo_position)
        {
            glDeleteBuffers(1, &vbo_position);
            vbo_position = 0;
        }

        if (vbo_color)
        {
            glDeleteBuffers(1, &vbo_color);
            vbo_color = 0;
        }

        if (vao)
        {
            glDeleteVertexArrays(1, &vao);
            vao = 0;
        }
    }
};