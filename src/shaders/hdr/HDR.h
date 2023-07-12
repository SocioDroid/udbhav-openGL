#pragma once
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLShaders.h"
#include "../../includes/vmath.h"
using namespace vmath;

static GLuint tempVao;
class HDR
{
private:
    GLuint FBO;
    GLuint RBO;
    GLuint colorTex;
    GLuint emisionTex;
    GLuint occlusionTex;
    GLfloat exposure;
    GLfloat fade;
    GLfloat threshhold;
    GLfloat knee;
    GLfloat bloom_intensity;
    GLuint mipLevels;
    GLsizei size;
    bool bloomEnabled;
    GLuint hdrprogram;
    GLuint downscaleprogram;
    GLuint upscaleprogram;

public:
    HDR(GLfloat exposure, GLfloat fade, GLsizei size)
    {
        threshhold = 1.5f;
        knee = 0.1f;
        bloom_intensity = 1.0f;
        bloomEnabled = false;

        // calculate max mip map levels
        GLuint width = size / 2;
        GLuint height = size / 2;
        this->mipLevels = 1;

        for (size_t i = 0; i < 16; i++)
        {
            width = width / 2;
            height = height / 2;

            if (width < 10 || height < 10)
                break;
            ++this->mipLevels;
        }
    }

    void setupProgram(void)
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // vertex Shader
        GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\hdr\\fsquad.vert", VERTEX);

        // fragment Shader
        GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\hdr\\hdr.frag", FRAGMENT);

        this->hdrprogram = glCreateProgram();
        glAttachShader(this->hdrprogram, vertexShaderObject);
        glAttachShader(this->hdrprogram, fragmentShaderObject);

        // link
        BOOL bShaderLinkStatus = LinkShaderProgramObject(this->hdrprogram);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        GLuint computeShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\hdr\\downscale.comp", COMPUTE);

        this->downscaleprogram = glCreateProgram();
        glAttachShader(this->downscaleprogram, computeShaderObject);

        // link
        bShaderLinkStatus = LinkShaderProgramObject(this->downscaleprogram);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        computeShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\hdr\\upscale.comp", COMPUTE);

        this->upscaleprogram = glCreateProgram();
        glAttachShader(this->upscaleprogram, computeShaderObject);

        // link
        bShaderLinkStatus = LinkShaderProgramObject(this->upscaleprogram);
    }

    void initialize(void)
    {

        glGenVertexArrays(1, &tempVao);
        glGenFramebuffers(1, &this->FBO);

        glGenTextures(1, &this->colorTex);
        glBindTexture(GL_TEXTURE_2D, this->colorTex);
        glTexStorage2D(GL_TEXTURE_2D, this->mipLevels, GL_RGBA32F, this->getSize(), this->getSize());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &this->emisionTex);
        glBindTexture(GL_TEXTURE_2D, this->emisionTex);
        glTexStorage2D(GL_TEXTURE_2D, this->mipLevels, GL_RGBA32F, this->getSize(), this->getSize());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenTextures(1, &this->occlusionTex);
        glBindTexture(GL_TEXTURE_2D, this->occlusionTex);
        glTexStorage2D(GL_TEXTURE_2D, this->mipLevels, GL_RGBA8, this->getSize(), this->getSize());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenRenderbuffers(1, &this->RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->getSize(), this->getSize());

        glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
        glBindTexture(GL_TEXTURE_2D, this->colorTex);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorTex, 0);

        glBindTexture(GL_TEXTURE_2D, this->emisionTex);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->emisionTex, 0);

        glBindTexture(GL_TEXTURE_2D, this->occlusionTex);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->occlusionTex, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->RBO);
        const GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glDrawBuffers(3, buffers);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            PrintLog("HDR Framebuffer Not Complete !!!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void render(void)
    {

        if (this->bloomEnabled)
        {
            // Bloom pass / Blur Pass
            // Down scale pass
            glUseProgram(this->downscaleprogram);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, this->emisionTex);
            vec2 mip_size = vec2(this->getSize() / 2, this->getSize() / 2);
            for (size_t i = 0; i < this->mipLevels - 1; ++i)
            {

                glUniform2fv(glGetUniformLocation(this->downscaleprogram, "u_texel_size"), 1, 1.0f / vec2(mip_size[0], mip_size[1]));
                glUniform1i(glGetUniformLocation(this->downscaleprogram, "u_mip_level"), i);
                glBindImageTexture(0, this->emisionTex, i + 1, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
                glDispatchCompute(ceil(float(mip_size[0]) / 8), ceil(float(mip_size[1]) / 8), 1);
                mip_size = mip_size / 2u;
                glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
            }

            // Up Scale Pass
            glUseProgram(this->upscaleprogram);
            glUniform1f(glGetUniformLocation(this->upscaleprogram, "u_bloom_intensity"), this->bloom_intensity);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, this->emisionTex);
            for (size_t i = this->mipLevels - 1; i >= 1; --i)
            {

                mip_size[0] = vmath::max(1.0f, float(this->getSize() / pow(2, i - 1)));
                mip_size[1] = vmath::max(1.0f, float(this->getSize() / pow(2, i - 1)));

                glUniform2fv(glGetUniformLocation(this->upscaleprogram, "u_texel_size"), 1, 1.0f / vec2(mip_size[0], mip_size[1]));
                glUniform1i(glGetUniformLocation(this->upscaleprogram, "u_mip_level"), i);
                glBindImageTexture(0, this->emisionTex, i - 1, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
                glDispatchCompute(ceil(float(mip_size[0]) / 8), ceil(float(mip_size[1]) / 8), 1);
                glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
            }
        }
        glUseProgram(this->hdrprogram);
        glUniform1i(glGetUniformLocation(this->hdrprogram, "hdrTex"), 0);
        glUniform1i(glGetUniformLocation(this->hdrprogram, "bloomTex"), 1);
        glUniform1f(glGetUniformLocation(this->hdrprogram, "exposure"), this->exposure);
        glUniform1f(glGetUniformLocation(this->hdrprogram, "fade"), this->fade);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->colorTex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->emisionTex);
        glBindVertexArray(tempVao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void setBloom(bool val)
    {
        this->bloomEnabled = val;
    }

    GLuint getFBO(void)
    {
        return this->FBO;
    }
    GLsizei getSize(void)
    {
        return this->size;
    }
    GLfloat getExposure(void)
    {
        return this->exposure;
    }
    void updateExposure(GLfloat delta)
    {
        this->exposure += delta;
    }
    void updateBloomIntensity(GLfloat delta)
    {
        this->bloom_intensity += delta;
    }

    void uninitialize(void)
    {
        glDeleteFramebuffers(1, &this->FBO);
        glDeleteRenderbuffers(1, &this->RBO);
        glDeleteTextures(1, &this->colorTex);
        glDeleteTextures(1, &this->emisionTex);
        glDeleteVertexArrays(1, &tempVao);
        UninitializeShaders(this->hdrprogram);
        UninitializeShaders(this->downscaleprogram);
        UninitializeShaders(this->upscaleprogram);
    }
};