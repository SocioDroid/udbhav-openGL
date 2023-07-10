#pragma once
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLShaders.h"
#include "../../shaders/bloom/Blur_Shader.h"
#include "../../shaders/bloom/BloomFinal_Shader.h"



class BloomShaders
{
    #define FBO_WIDTH_BLOOM 1920
    #define FBO_HEIGHT_BLOOM 1080

public:
    // Variables
    GLuint vao;          // Vertex Array Object -
    GLuint vbo_Position; // Vertex Buffer Object - Pyramid - Position
    GLuint vbo_Texcoord; // Vertex Buffer Object - Pyramid
    GLuint vbo_Normal;   // Vertex Buffer Object - Pyramid

    // Bloom Related Variables
    float exposure = 0.0f;

    unsigned int hdrFBO;
    unsigned int rboDepth;
    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];
    unsigned int colorBuffers[2];

    // Blur Shader
    BlurShader blurShader;
    BloomFinalShader bloomFinalShader;
    bool horizontal = true;

    BOOL initialize_bloomShaderObject(void)
    {

        // Shaders
        blurShader.initialize();
        bloomFinalShader.initialize();

        createFBO();

        const GLfloat position[] = {
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f};

        const GLfloat texCoord[] = {
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f};

        // vao and vbo related code
        // vao for QUAD
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // vbo for position
        glGenBuffers(1, &vbo_Position);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_Position);
        glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // vbo for Texcoord
        glGenBuffers(1, &vbo_Texcoord);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_Texcoord);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);
        glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        glUseProgram(blurShader.shaderProgramObject);
        glUniform1i(blurShader.textureSamplerUniform, 0);
        glUseProgram(0);

        return TRUE;
    }

    void display()
    {
    }

    void uninitialize_BloomShaderObject(void)
    {
    }

    void *funPtr = NULL;

    // void renderSceneToBeBloomed(void (*fpnScene)())
    // {
    //     // code
    //     glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    //     {
    //         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //         // resizeBloomScene(giWindowWidth, giWindowHeight);
    //         // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //         glViewport(0, 0, FBO_WIDTH, FBO_HEIGHT);
    //         fpnScene();
    //     }
    //     glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // }

    void bindBloomFBO(void)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, FBO_WIDTH_BLOOM, FBO_HEIGHT_BLOOM);
    }

    void unbindBloomFBO(void)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void renderBlurFBO(void)
    {
        // 2. blur bright fragments with two-pass Gaussian Blur
        bool first_iteration = true;
        unsigned int amount = 10;

        glUseProgram(blurShader.shaderProgramObject);
        {
            // --------------------------------------------------
            for (unsigned int i = 0; i < amount; i++)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
                glViewport(0, 0, FBO_WIDTH_BLOOM, FBO_HEIGHT_BLOOM);
                glUniform1i(blurShader.isHorizontalUniform, horizontal);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]); // bind texture of other framebuffer (or scene if first iteration)

                // glEnable(GL_BLEND);
                // glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glBindVertexArray(0);
                // glDisable(GL_BLEND);

                glBindTexture(GL_TEXTURE_2D, 0);

                horizontal = !horizontal;
                if (first_iteration)
                    first_iteration = false;
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        glUseProgram(0);
    }

    void renderFinalBloomScene(GLuint texture_scene)
    {
        //code
        glClearColor(0.0 ,0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glViewport(0, 0, giWindowWidth, giWindowHeight);
        // PrintLog("%d, %d\n", giWindowWidth, giWindowHeight);

        glUseProgram(bloomFinalShader.shaderProgramObject);
        {
            glUniformMatrix4fv(bloomFinalShader.projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
            glUniformMatrix4fv(bloomFinalShader.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
            glUniformMatrix4fv(bloomFinalShader.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, texture_scene);

            glUniform1i(bloomFinalShader.isBloomUniform, TRUE);
            glUniform1f(bloomFinalShader.exposureUniform, exposure);
            // glUniform1f(bloomFinalShader.exposureUniform, 15.0);

            glUniform1i(bloomFinalShader.sceneTextureSamplerUniform, 0);
            glUniform1i(bloomFinalShader.blurTextureSamplerUniform, 1);
            glUniform1i(bloomFinalShader.noBloomSceneTextureSamplerUniform, 2);

            //glEnable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE);

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindVertexArray(0);

            //glDisable(GL_BLEND);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glUseProgram(0);
    }

    void createFBO(void)
    {
        glGenFramebuffers(1, &hdrFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        // create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)

        glGenTextures(2, colorBuffers);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, FBO_WIDTH_BLOOM, FBO_HEIGHT_BLOOM, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            // attach texture to framebuffer
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
        }

        // create and attach depth buffer (renderbuffer)
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, FBO_WIDTH_BLOOM, FBO_HEIGHT_BLOOM);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

        // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
        unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, attachments);
        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            PrintLog("Framebuffer not completed\n");
        }
        //	std::cout << "Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenFramebuffers(2, pingpongFBO);
        glGenTextures(2, pingpongColorbuffers);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
            glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, FBO_WIDTH_BLOOM, FBO_HEIGHT_BLOOM, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
            // also check if framebuffers are complete (no need for depth buffer)
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                PrintLog("Framebuffer for Blur not completed\n");
            }
            //	std::cout << "Framebuffer not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};