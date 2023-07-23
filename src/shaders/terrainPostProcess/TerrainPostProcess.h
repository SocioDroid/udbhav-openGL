#pragma once
#include <string>
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLShaders.h"
#include "../../includes/vmath.h"
using namespace vmath;

class TerrainPostProcess
{
public:
    // Variables
    /* SKY RELATED VARIALBES*/
    GLuint shaderProgramObject;
    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;
    bool initialized = false;

    BOOL initialize(void)
    {
        initializeQuad();

        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\terrainPostProcess\\screen.vert", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\terrainPostProcess\\post_processing.frag", FRAGMENT);

        // Shader Program Object
        shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject);

        if (bShaderLinkStatus == FALSE)
            return FALSE;

        return TRUE;
    }

    void initializeQuad()
    {
        if (!initialized)
        {
            float vertices[] = {
                -1.0f, -1.0f, 0.0, 0.0,
                1.0f, -1.0f, 1.0, 0.0,
                -1.0f, 1.0f, 0.0, 1.0,
                1.0f, 1.0f, 1.0, 1.0,
                -1.0f, 1.0f, 0.0, 1.0,
                1.0f, -1.0f, 1.0, 0.0};

            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);
            initialized = true;
        }
    }
    void draw()
    {
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    static void disableTests()
    {
        // glDisable(GL_CLIP_DISTANCE0);
        glDisable(GL_DEPTH_TEST);
    }

    static void enableTests()
    {
        glEnable(GL_DEPTH_TEST);
    }

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgramObject, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgramObject, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(shaderProgramObject, name.c_str()), value);
    }
    void setVec2(const std::string &name, vmath::vec2 vector) const
    {
        unsigned int location = glGetUniformLocation(shaderProgramObject, name.c_str());
        glUniform2fv(location, 1, vector);
    }
    void setVec3(const std::string &name, vmath::vec3 vector) const
    {
        unsigned int location = glGetUniformLocation(shaderProgramObject, name.c_str());
        glUniform3fv(location, 1, vector);
    }

    void setVec4(const std::string &name, vmath::vec4 vector) const
    {
        unsigned int location = glGetUniformLocation(shaderProgramObject, name.c_str());
        glUniform4fv(location, 1, vector);
    }

    void setMat4(const std::string &name, vmath::mat4 matrix) const
    {
        unsigned int mat = glGetUniformLocation(shaderProgramObject, name.c_str());
        glUniformMatrix4fv(mat, 1, false, matrix);
    }

    void setSampler2D(const std::string &name, unsigned int texture, int id) const
    {
        glActiveTexture(GL_TEXTURE0 + id);
        glBindTexture(GL_TEXTURE_2D, texture);
        this->setInt(name, id);
    }
    void setSampler3D(const std::string &name, unsigned int texture, int id) const
    {
        glActiveTexture(GL_TEXTURE0 + id);
        glBindTexture(GL_TEXTURE_3D, texture);
        this->setInt(name, id);
    }

    void uninitialize(void)
    {
        UninitializeShaders(shaderProgramObject);
    }
};
