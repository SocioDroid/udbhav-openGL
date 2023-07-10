#pragma once
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLShaders.h"
#include "../../includes/vmath.h"
using namespace vmath;
class TextureDisintegrateShader
{
public:
    // Variables
    /* SKY RELATED VARIALBES*/
    GLuint shaderProgramObject;

    GLuint projectionMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint modelMatrixUniform;
    GLuint textureSamplerUniform;
    GLuint timeUniform;
    GLuint alphaValueUniform;
    GLuint vao;
    GLuint vbo_position;
    GLuint vbo_texcoords;

    GLuint resolutionX;
    GLuint resolutionY;
    GLuint clearCMixUniform;

    BOOL initialize(void)
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects("./src/shaders/textureDisintegrate/texture.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects("./src/shaders/textureDisintegrate/texture.fs", FRAGMENT);

        shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // prelinked binding
        // Binding Position Array
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_POSITION, "a_position");
        // Binding Color Array
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_TEXTURE0, "a_texcoord");

        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject);

        if (bShaderLinkStatus == FALSE)
            return FALSE;

        // post link - getting
        projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
        viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
        modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");
        textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");
        alphaValueUniform = glGetUniformLocation(shaderProgramObject, "u_alphaVal");
        timeUniform = glGetUniformLocation(shaderProgramObject, "u_iTime");
        resolutionX = glGetUniformLocation(shaderProgramObject, "resX");
        resolutionY = glGetUniformLocation(shaderProgramObject, "resY");
        clearCMixUniform = glGetUniformLocation(shaderProgramObject, "u_clearCMix");

        // VAO For QUAD

        const GLfloat positions[] =
            {
                // front
                1.0f, 1.0f, 0.0f,
                -1.0f, 1.0f, 0.0f,
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f};

        const GLfloat texcoord[] =
            {
                1.0, 0.0,
                0.0, 0.0,
                0.0, 1.0,
                1.0, 1.0};

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        {
            glGenBuffers(1, &vbo_position);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
            glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
            glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glGenBuffers(1, &vbo_texcoords);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoords);
            glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord), texcoord, GL_STATIC_DRAW);
            glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        glBindVertexArray(0);

        return TRUE;
    }

    void drawQuadWithTexture(GLuint texture, mat4 modelMatrix, mat4 viewMatrix, mat4 perspectiveProjectionMatrix, float alpha, float time, float objX, float objY, float clearCMix)
    {
        glUseProgram(shaderProgramObject);
        {
            // Uniforms
            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
            glUniform1f(alphaValueUniform, alpha);
            glUniform1f(timeUniform, time);

            glUniform1f(resolutionX, 1920.0f + objX);
            glUniform1f(resolutionY, 1080.0f + objY);

            glUniform1f(timeUniform, time);
            glUniform1i(textureSamplerUniform, 0);
            glUniform1f(clearCMixUniform, clearCMix);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // 4 Vertices for cube
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_BLEND);
        }
        glUseProgram(0);
    }
    void uninitialize(void)
    {
        UninitializeShaders(shaderProgramObject);
    }
};
