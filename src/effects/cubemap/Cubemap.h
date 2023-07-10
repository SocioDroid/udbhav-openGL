#pragma once

#include "../../utils/common.h"
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLLog.h"
#include "../../shaders/cubemap/CubemapShader.h"

class CubeMap
{
public:
    CubemapShader *cubemap = NULL;
    Cube *cube = NULL;
    GLuint vao_cube;
    GLuint vbo_cube_position;
    unsigned int cubemapTexture;
    int isBarasat = 0;

    BOOL initialize(const char *faces_path[])
    {
        // code
        cubemap = new CubemapShader();
        if (cubemap->initialize() == FALSE)
        {
            PrintLog("Failed to initialize CubeMap\n");
            return FALSE;
        }

        const GLfloat cubeVertices[] =
            {
                // positions          // normals
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

        // vao
        glGenVertexArrays(1, &vao_cube);
        glBindVertexArray(vao_cube);
        // vbo for position
        glGenBuffers(1, &vbo_cube_position);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_position);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              6 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);

        glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              6 * sizeof(float),
                              (void *)(3 * sizeof(float)));

        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind position vbo

        glBindVertexArray(0); // unbind vao

        // char* faces[] =
        // {
        //         "./assets/textures/hall/right.jpg",
        //         "./assets/textures/hall/left.jpg",
        //         "./assets/textures/hall/top.jpg",
        //         "./assets/textures/hall/bottom.jpg",
        //         "./assets/textures/hall/front.jpg",
        //         "./assets/textures/hall/back.jpg"
        // };
        cubemapTexture = loadCubemap(faces_path);

        return TRUE;
    }

    void display(void)
    {
        // code
        glUseProgram(cubemap->shaderProgramObject);

        glUniformMatrix4fv(cubemap->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(cubemap->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(cubemap->projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
        glUniform1i(cubemap->isBarsatUniform, isBarasat);
        glBindVertexArray(vao_cube);

        // Game coding here
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glUseProgram(0);
    }

    void uninitialize(void)
    {
        // code
    }
};
