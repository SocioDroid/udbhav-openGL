#pragma once
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLShaders.h"
#include "../../includes/vmath.h"
using namespace vmath;
class NoiseCloudShader
{
public:
    // Variables
    /* SKY RELATED VARIALBES*/
    GLuint shaderProgramObject;

    GLuint projectionMatrixUniform; // model View Projection
    GLuint viewMatrixUniform;    // model View Projection
    GLuint modelMatrixUniform;   // model View Projection

    GLuint lightPosUniform;
    GLuint scaleUniform;
    GLuint Color1Uniform;
    GLuint Color2Uniform;

    GLuint textureSamplerUniform;

    float lightPos[4] = {0.0f, 0.0f, 4.0f, 1.0f};

    float Color1[3] = {0.8f, 0.7f, 0.0f};
    float Color2[3] = {0.6f, 0.1f, 0.0f};

    GLfloat scaleFactor = 2.0f;
    BOOL isScaled = FALSE;

    BOOL initialize(void)
    {
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects("./src/shaders/noiseCloud/noise.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects("./src/shaders/noiseCloud/noise.fs", FRAGMENT);

        shaderProgramObject = glCreateProgram();
        glAttachShader(shaderProgramObject, vertexShaderObject);
        glAttachShader(shaderProgramObject, fragmentShaderObject);

        // prelinked binding
        // Binding Position Array
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_POSITION, "a_position");
        // Binding Color Array
        glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_NORMAL, "a_normal");

        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject);

        if (bShaderLinkStatus == FALSE)
            return FALSE;
        // post link - getting
        projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
        viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
        modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");

        lightPosUniform = glGetUniformLocation(shaderProgramObject, "LightPos");
        scaleUniform = glGetUniformLocation(shaderProgramObject, "NoiseScale");

        textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");

        Color1Uniform = glGetUniformLocation(shaderProgramObject, "Color1"); // Sky related uniform
        Color2Uniform = glGetUniformLocation(shaderProgramObject, "Color2"); // Sky related uniform
        return TRUE;
    }

    void update(void)
    {
        glUniform3fv(lightPosUniform, 1, lightPos);
        glUniform1f(scaleUniform, scaleFactor);

        glUniform3fv(Color1Uniform, 1, Color1);
        glUniform3fv(Color2Uniform, 1, Color2);

        if (isScaled == TRUE)
        {
            scaleFactor = scaleFactor - 0.005f;

            if (scaleFactor < 1.0f)
                isScaled = FALSE;
        }
        else
        {
            // scaleFactor = scaleFactor + 0.1f;
            scaleFactor = scaleFactor + 0.005f;
            if (scaleFactor > 2.0f)
                isScaled = TRUE;
        }
    }

    void uninitialize(void)
    {
        UninitializeShaders(shaderProgramObject);
    }
};
