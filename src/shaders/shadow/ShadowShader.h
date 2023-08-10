#pragma once
#include "../../utils/camera/Camera.h"
#include "../../shaders/model/Model_ShaderShadow.h"
#include "../../utils/common.h"

extern Camera camera;

class ShadowShader
{
public:
    // Variables
    GLuint shaderProgramObject_depth;
    GLuint shaderProgramObject_shadow;
    GLuint shaderProgramObject_debugQuad;

    GLuint modelMatrixUniform_depth;
    GLuint viewMatrixUniform_depth;
    GLuint projectionMatrixUniform_depth;

    GLuint modelMatrixUniform_shadow;
    GLuint viewMatrixUniform_shadow;
    GLuint projectionMatrixUniform_shadow;
    GLuint lightSpaceMatrixUniform_shadow;
    GLuint diffuseTextureUniform_shadow;
    GLuint shadowMapTextureUniform_shadow;
    GLuint lightPosUniform_shadow;
    GLuint viewPosUniform_shadow;

    GLuint depthMapTextureUniform_debudQuad;
    GLuint nearPlaneUniform_debudQuad;
    GLuint farPlaneUniform_debudQuad;

    GLint giWidth;
    GLint giHeight;

    GLuint planeVAO;
    GLuint planeVBO;

    GLuint quadVAO = 0;
    GLuint quadVBO;

    GLuint cubeVAO = 0;
    GLuint cubeVBO = 0;

    GLuint depthMapFBO;
    GLuint depthMapTexture;

    float lightPos[3] = {0.0f, 5.0f, 2.0f};

    float lightDirection[3] = {1.0f, 0.0f, 0.0f};

    Cube *cube = nullptr;

    GLuint SHADOW_WIDTH = 10000;
    GLuint SHADOW_HEIGHT = 10000;

    BOOL initialize()
    {

        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\shadow\\shadow_mapping_depth.vs", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\shadow\\shadow_mapping_depth.fs", FRAGMENT);

        shaderProgramObject_depth = glCreateProgram();
        glAttachShader(shaderProgramObject_depth, vertexShaderObject);
        glAttachShader(shaderProgramObject_depth, fragmentShaderObject);

        // prelinked binding
        // Binding Position Array
        glBindAttribLocation(shaderProgramObject_depth, MATRIX_ATTRIBUTE_POSITION, "a_position");
        // Binding Color Array
        glBindAttribLocation(shaderProgramObject_depth, MATRIX_ATTRIBUTE_NORMAL, "a_normal");

        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject_depth);

        if (bShaderLinkStatus == FALSE)
            return FALSE;
        // post link - getting
        modelMatrixUniform_depth = glGetUniformLocation(shaderProgramObject_depth, "u_modelMatrix");
        // viewMatrixUniform_depth = glGetUniformLocation(shaderProgramObject_depth, "u_viewMatrix");
        projectionMatrixUniform_depth = glGetUniformLocation(shaderProgramObject_depth, "u_lightSpaceMatrix");

        // Declaration of vertex data array
        float planeVertices[] = {
            // positions            // normals         // texcoords
            25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        // plane VAO

        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glBindVertexArray(0);

        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
            1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left
            // front face
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
            -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
            -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
                                                                // right face
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
            1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top-right
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // bottom-left
            // bottom face
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
            -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
            1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // bottom-left
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            0.0f,
            1.0f,
            1.0f,
            1.0f,
            -1.0f,
            0.0f,
            1.0f,
            0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Shaders for ShadowMap
        {
            GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\shadow\\shadow_mapping.vs", VERTEX);
            if (vertexShaderObject == 0)
            {
                PrintLog("Failed to compile VertexShader for ShadowMap\n");
                uninitialize();
            }

            GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\shadow\\shadow_mapping.fs", FRAGMENT);
            if (fragmentShaderObject == 0)
            {
                PrintLog("Failed to compile FragmentShader for ShadowMap\n");
                uninitialize();
            }
            // shader program object
            shaderProgramObject_shadow = glCreateProgram();

            glAttachShader(shaderProgramObject_shadow, vertexShaderObject);
            glAttachShader(shaderProgramObject_shadow, fragmentShaderObject);

            // prelinked binding
            glBindAttribLocation(shaderProgramObject_shadow, MATRIX_ATTRIBUTE_POSITION, "aPos");
            glBindAttribLocation(shaderProgramObject_shadow, MATRIX_ATTRIBUTE_TEXTURE0, "aTexCoords");
            glBindAttribLocation(shaderProgramObject_shadow, MATRIX_ATTRIBUTE_NORMAL, "aNormal");

            // Linking the shaderProgramObject_shadow
            if (LinkShaderProgramObject(shaderProgramObject_shadow) == FALSE)
            {
                return (-6);
            }
            //
            modelMatrixUniform_shadow = glGetUniformLocation(shaderProgramObject_shadow, "u_modelMatrix");
            viewMatrixUniform_shadow = glGetUniformLocation(shaderProgramObject_shadow, "u_viewMatrix");
            projectionMatrixUniform_shadow = glGetUniformLocation(shaderProgramObject_shadow, "u_projectionMatrix");
            lightSpaceMatrixUniform_shadow = glGetUniformLocation(shaderProgramObject_shadow, "u_lightSpaceMatrix");

            diffuseTextureUniform_shadow = glGetUniformLocation(shaderProgramObject_shadow, "diffuseTexture");
            shadowMapTextureUniform_shadow = glGetUniformLocation(shaderProgramObject_shadow, "shadowMap");

            lightPosUniform_shadow = glGetUniformLocation(shaderProgramObject_shadow, "lightPos");
            viewPosUniform_shadow = glGetUniformLocation(shaderProgramObject_shadow, "viewPos");
        }

        // Shaders for Debug Quad
        {
            GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\shadow\\debug_quad.vs", VERTEX);
            if (vertexShaderObject == 0)
            {
                PrintLog("Failed to compile VertexShader for Debug Quad\n");
                uninitialize();
            }

            GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\shadow\\debug_quad.fs", FRAGMENT);
            if (fragmentShaderObject == 0)
            {
                PrintLog("Failed to compile FragmentShader for Debug Quad\n");
                uninitialize();
            }
            // shader program object
            shaderProgramObject_debugQuad = glCreateProgram();

            glAttachShader(shaderProgramObject_debugQuad, vertexShaderObject);
            glAttachShader(shaderProgramObject_debugQuad, fragmentShaderObject);

            // prelinked binding
            glBindAttribLocation(shaderProgramObject_debugQuad, MATRIX_ATTRIBUTE_POSITION, "aPos");
            glBindAttribLocation(shaderProgramObject_debugQuad, MATRIX_ATTRIBUTE_TEXTURE0, "aTexCoords");

            // Linking the shaderProgramObject_debugQuad
            if (LinkShaderProgramObject(shaderProgramObject_debugQuad) == FALSE)
            {
                return (-6);
            }
            //
            depthMapTextureUniform_debudQuad = glGetUniformLocation(shaderProgramObject_debugQuad, "depthMap");
            nearPlaneUniform_debudQuad = glGetUniformLocation(shaderProgramObject_debugQuad, "near_plane");
            farPlaneUniform_debudQuad = glGetUniformLocation(shaderProgramObject_debugQuad, "far_plane");
        }

        // Create Depth FBO
        createFBO();
        return TRUE;
    }

    void createFBO(void)
    {
        // Create FBO
        glGenFramebuffers(1, &depthMapFBO);

        // create depth texture
        glGenTextures(1, &depthMapTexture);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        float borderColor[] = {1.0, 1.0, 1.0, 1.0};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
        glDrawBuffer(GL_NONE);
        // glReadBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            PrintLog("Framebuffer Incomplete\n");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        PrintLog("----------  exiting CreateFBO  -----------------\n");
    }

    void renderMainScene(void)
    {

        mat4 lightProjectionMatrix = mat4::identity();
        mat4 lightViewMatrix = mat4::identity();
        mat4 lightSpaceMatrix = mat4::identity();
        float near_plane = 1.0f, far_plane = 7.5f;

        lightPos[0] = 1.600000f;
        lightPos[1] = 5.599997f;
        lightPos[2] = 4.599998f;

        lightDirection[0] = 0.0f;
        lightDirection[1] = 0.0f;
        lightDirection[2] = 0.0f;

        // PrintLog("%f, %f, %f\n%f, %f, %f\n\n", lightPos[0], lightPos[1], lightPos[2], lightDirection[0], lightDirection[1], lightDirection[2]);
        lightProjectionMatrix = vmath::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
        lightViewMatrix = vmath::lookat(vec3(lightPos[0], lightPos[1], lightPos[2]), vec3(lightDirection[0], lightDirection[1], lightDirection[2]), vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use Depth Shader
        glUseProgram(shaderProgramObject_depth);
        {

            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            {
                glUniformMatrix4fv(projectionMatrixUniform_depth, 1, GL_FALSE, lightSpaceMatrix);
                // glUniformMatrix4fv(viewMatrixUniform_depth, 1, GL_FALSE, lightViewMatrix);
                mat4 translateMatrix = mat4::identity();
                mat4 rotateMatrix = mat4::identity();

                translateMatrix = vmath::translate(2.0f + -2.0f, -0.5f + -0.439998f, 34.50f - 30.500000f + -1.0f);
                rotateMatrix = vmath ::rotate(65.50f, 0.0f, 1.0f, 0.0f);

                modelMatrix = translateMatrix * rotateMatrix;

                glUniformMatrix4fv(modelMatrixUniform_depth, 1, GL_FALSE, modelMatrix);

                glClear(GL_DEPTH_BUFFER_BIT);

                // awara->displayForShadow(modelMatrixUniform_depth, shaderProgramObject_depth, TRUE, lightSpaceMatrix, vec3(lightPos[0], lightPos[1], lightPos[2]));
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        glUseProgram(0);

        // {
        //     modelMatrix = mat4::identity();

        //     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //     glViewport(0, 0, giWindowWidth, giWindowHeight);

        //     perspectiveProjectionMatrix = vmath::perspective(
        //         45.0f,
        //         (GLfloat)giWindowWidth / (GLfloat)giWindowHeight,
        //         0.1f,
        //         1000.0f);

        //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //     if (USE_FPV_CAM)
        //     {
        //         viewMatrix = camera.getViewMatrix();
        //     }
        //     else
        //     {
        //         viewMatrix = globalBezierCamera->getViewMatrix();
        //     }

        //     // Use default shader + shadow Shader
        //     glUseProgram(shaderProgramObject_shadow);
        //     {
        //         // vec3 viewPos = vec3(0.0f + objX, 0.0f + objY, 10.0f + objZ);
        //         // vec3 viewPos = vec3(0.0f, 3.0f, 10.0f);
        //         vec3 viewPos;
        //         if (USE_FPV_CAM)
        //         {
        //             viewPos = camera.getEye();
        //         }
        //         else
        //         {
        //             viewPos = globalBezierCamera->getEye();
        //         }

        //         glUniformMatrix4fv(projectionMatrixUniform_shadow, 1, GL_FALSE, perspectiveProjectionMatrix);
        //         glUniformMatrix4fv(viewMatrixUniform_shadow, 1, GL_FALSE, viewMatrix);
        //         glUniformMatrix4fv(lightSpaceMatrixUniform_shadow, 1, GL_FALSE, lightSpaceMatrix);
        //         glUniform3fv(viewPosUniform_shadow, 1, viewPos);
        //         glUniform3fv(lightPosUniform_shadow, 1, lightPos);

        //         // pushMatrix(modelMatrix);
        //         {
        //             awara->display(FALSE, depthMapTexture, lightSpaceMatrix, vec3(lightPos[0], lightPos[1], lightPos[2]));
        //         }

        //         // Unbind Texture
        //         glActiveTexture(GL_TEXTURE1);
        //         glBindTexture(GL_TEXTURE_2D, 0);
        //         glActiveTexture(GL_TEXTURE0);
        //         glBindTexture(GL_TEXTURE_2D, 0);
        //     }
        //     glUseProgram(0);
        // }
    }
    void renderQuad()
    {
        if (quadVAO == 0)
        {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,
                1.0f,
                0.0f,
                0.0f,
                1.0f,
                -1.0f,
                -1.0f,
                0.0f,
                0.0f,
                0.0f,
                1.0f,
                1.0f,
                0.0f,
                1.0f,
                1.0f,
                1.0f,
                -1.0f,
                0.0f,
                1.0f,
                0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

    void display(void)
    {
        renderMainScene();
    }

    void uninitialize(void)
    {
        if (planeVBO)
        {
            glDeleteBuffers(1, &planeVBO);
            planeVBO = 0;
        }

        // Deletion/uninitialization of VAO
        if (planeVAO)
        {
            glDeleteVertexArrays(1, &planeVAO);
            planeVAO = 0;
        }

        UninitializeShaders(shaderProgramObject_debugQuad);
        UninitializeShaders(shaderProgramObject_shadow);
        UninitializeShaders(shaderProgramObject_depth);
    }
};
