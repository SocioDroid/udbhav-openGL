#pragma once

#include "../../utils/common.h"
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLLog.h"
#include "../../shaders/cubemapMerge/CubemapShaderMerge.h"

class CubeMapMerge
{
public:
    CubemapShaderMerge *cubemap = NULL;
    Cube *cube = NULL;
    GLuint vao_cube;
    GLuint vbo_cube_position;
    unsigned int cubemapTexture1;
    unsigned int cubemapTexture2;
    int isBarasat = 0;

    BOOL initialize(const char *faces_path1[], const char *faces_path2[])
    {
        // code
        cubemap = new CubemapShaderMerge();
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

        cubemapTexture1 = loadCubemap(faces_path1);
        cubemapTexture2 = loadCubemap(faces_path2);

        return TRUE;
    }

    void display(float factor)
    {
        // code
        glUseProgram(cubemap->shaderProgramObject);

        glUniformMatrix4fv(cubemap->modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(cubemap->viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(cubemap->projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
        glUniform1f(cubemap->integrateFactorUniform, factor);
        glUniform1i(cubemap->skyBoxUniform1, 0);
        glUniform1i(cubemap->skyBoxUniform2, 1);
        glBindVertexArray(vao_cube);

        // Game coding here
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture2);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glUseProgram(0);
    }

    void uninitialize(void)
    {
        // code
    }
};
