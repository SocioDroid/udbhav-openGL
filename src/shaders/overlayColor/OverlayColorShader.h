#pragma once
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLShaders.h"
#include "../../includes/vmath.h"
using namespace vmath;

class OverlayColorShader
{
public:
    // Variables
    /* SKY RELATED VARIALBES*/
    GLuint shaderProgramObject;

    GLuint mvpMatrixUniform;
    GLuint textureSamplerUniform;
    GLuint alphaValueUniform;
    GLuint vao;
    GLuint vbo_position;
    GLuint vbo_texcoords;
    BOOL initialize(void)
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects("./src/shaders/overlayColor/color.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects("./src/shaders/overlayColor/color.fs", FRAGMENT);

        shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // prelinked binding
        // Binding Position Array
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_POSITION, "a_position");
        // Binding Color Array
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_COLOR, "a_color");

        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject);

        if (bShaderLinkStatus == FALSE)
            return FALSE;

        // post link - getting
        mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");

        // VAO For QUAD

        const GLfloat positions[] =
            {
                // front
                1.0f, 1.0f, 0.0f,
                -1.0f, 1.0f, 0.0f,
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f};

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        {
            glGenBuffers(1, &vbo_position);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
            glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
            glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        glBindVertexArray(0);

        return TRUE;
    }

    void draw(vmath::mat4 mvpMatrix, float r, float g, float b, float a)
    {
        glUseProgram(shaderProgramObject);
        {
            // Uniforms
            glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, mvpMatrix);
            glUniform1i(textureSamplerUniform, 0);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBindVertexArray(vao);

            glVertexAttrib4f(MATRIX_ATTRIBUTE_COLOR, r, g, b, a);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // 4 Vertices for cube
            glBindVertexArray(0);
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
        }
        glUseProgram(0);
    }

    void uninitialize(void)
    {
        UninitializeShaders(shaderProgramObject);
    }
};
