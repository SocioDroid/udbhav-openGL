#pragma once
#include "../../utils/OpenGL/GLHeadersAndMacros.h"
#include "../../utils/OpenGL/GLShaders.h"
#include "../../utils/common.h"

#define REFLECTION_FBO_WIDTH 1920
#define REFLECTION_FBO_HEIGHT 1080

class ReflectionShader
{
public:
    // Variables


    float fEyeX = 0.0f;
    float fEyeY = 0.0f;
    float fEyeZ = 3.0f;

    vec3 eyePosition = vmath::vec3(fEyeX, fEyeY, fEyeZ);
    vec3 center = vmath::vec3(0.0f, 0.0f, 0.0f);
    vec3 up = vmath::vec3(0.0f, 1.0f, 0.0f);

    //---Surface Quad Reflection---
    GLuint vao_SurfaceQuad;
    GLuint vbo_SurfaceQuad_Position;
    GLuint vbo_SurfaceQuad_Texcoords;

    GLuint shaderProgramObject_SurfaceQuad;
    GLuint modelMatrixUniform_SurfaceQuad;
    GLuint viewMatrixUniform_SurfaceQuad;
    GLuint projectionMatrixUniform_SurfaceQuad;

    GLuint reflectionTextureSampler_SurfaceQuad;
    vec3 cameraPositions = vec3(0.0f, 1.0f, 2.0f);
    vec3 centerPositions = vec3(0.0f, 0.0f, 0.0f);
    vec3 upVectorPositions = vec3(0.0f, 1.0f, 0.0f);

    //FBO related variables:
    GLuint fbo_reflection;
    GLuint rbo_reflection;
    GLuint fbo_texture_reflection;


    BOOL initialize(void)
    {
        //prototypes:

        //code:

        BOOL status=FALSE;
        
        status=initialize_SurfaceQuad();
        if(status==FALSE)
        {
            PrintLog("initialize_SurfaceQuad Failed.");
            return status;
        }

        status=initializeReflectionFBO(REFLECTION_FBO_WIDTH,REFLECTION_FBO_HEIGHT);
        if(status==FALSE)
        {
            PrintLog("initializeReflectionFBO Failed.");
            return status;
        }else
        {
            PrintLog("initializeReflectionFBO Success.");
        }

        return TRUE;
    }

    BOOL initialize_SurfaceQuad(void)
    {
        //local:
        GLuint vertexShaderObject;
        GLuint fragmentShaderObject;

        //code:
        vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\environmentMap\\surfaceReflect.vs", VERTEX);
        if(vertexShaderObject==0)
        {
            return (SHADER_OBJECT_ERROR);
        }

        fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\environmentMap\\surfaceReflect.fs", FRAGMENT);
        if(fragmentShaderObject==0)
        {
            return (SHADER_OBJECT_ERROR);
        }

        shaderProgramObject_SurfaceQuad = glCreateProgram();

        glAttachShader(shaderProgramObject_SurfaceQuad, vertexShaderObject);
        glAttachShader(shaderProgramObject_SurfaceQuad, fragmentShaderObject);

        glBindAttribLocation(shaderProgramObject_SurfaceQuad, MATRIX_ATTRIBUTE_POSITION, "a_position");  //Andhar

        if (LinkShaderProgramObject(shaderProgramObject_SurfaceQuad) != TRUE)
        {
            PrintLog("\n# LinkShaderProgramObject() FAILED . ");
            return (SHADER_PROGRAM_OBJECT_ERROR);
        }
        else {
            PrintLog("\n# LinkShaderProgramObject() SUCCESSFUL . ");
        }

	    //post link + error checking
        modelMatrixUniform_SurfaceQuad = glGetUniformLocation(shaderProgramObject_SurfaceQuad, "u_modelMatrix");
        viewMatrixUniform_SurfaceQuad = glGetUniformLocation(shaderProgramObject_SurfaceQuad, "u_viewMatrix");
        projectionMatrixUniform_SurfaceQuad = glGetUniformLocation(shaderProgramObject_SurfaceQuad, "u_projectionMatrix");  //aandhar

        //reflectionTextureSampler_SurfaceQuad = glGetUniformLocation(shaderProgramObject_SurfaceQuad, "u_reflectionTextureSampler");  //aandhar

        const GLfloat surfaceQuadPositions[] = 
            {
                -2.0f,-2.0f,0.0f,//III
                -2.0f,2.0f,0.0f,//II
                2.0f,-2.0f,0.0f,//IV

                2.0f,-2.0f,0.0f,//IV
                -2.0f,2.0f,0.0f,//II
                2.0f,2.0f,0.0f//I
            };

    	//#----QUAD-----#
        glGenVertexArrays(1, &vao_SurfaceQuad);
        glBindVertexArray(vao_SurfaceQuad);
        {
            glGenBuffers(1, &vbo_SurfaceQuad_Position);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_SurfaceQuad_Position);
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(surfaceQuadPositions), surfaceQuadPositions, GL_STATIC_DRAW);
                glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
                glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        glBindVertexArray(0);


        return (TRUE);
    }


    BOOL initializeReflectionFBO(GLint reflectionTextureWidth,GLint reflectionTextureHeight)
    {
        //local:
        int maxRenderbufferSize;

        //code:
        glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,&maxRenderbufferSize);

        // # Step 1 : Check Available Render Buffer Size : 
        if(maxRenderbufferSize<reflectionTextureWidth || maxRenderbufferSize<reflectionTextureHeight)
        {
            PrintLog("\n #Error: Reflection Insufficient Render Buffer Size . ");
            return false;
        }

        //# Step 2 : Create Frame Buffer Object : 
        glGenFramebuffers(1,&fbo_reflection);
        glBindFramebuffer(GL_FRAMEBUFFER,fbo_reflection);
            //# Create render buffer object :
            glGenRenderbuffers(1,&rbo_reflection);
            glBindRenderbuffer(GL_RENDERBUFFER,rbo_reflection);
            //# Where to keep this Render Buffer+ What will be Format of it 
            //# i.e STORAGE and FORMAT of RenderBuffer.
            glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,reflectionTextureWidth,reflectionTextureHeight);
            //# Create Empty Texture for Upcoming Target Scene : 
            glGenTextures(1,&fbo_texture_reflection);
            glBindTexture(GL_TEXTURE_2D,fbo_texture_reflection);

            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,reflectionTextureWidth,reflectionTextureHeight,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
            //# Give above texture to FBO : 
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbo_texture_reflection,0);
            //# Give Render buffer to FBO :
            glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rbo_reflection);

            //# Check whether the FrameBuffer is created successfully or NOT :
            GLenum result=glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if(result!=GL_FRAMEBUFFER_COMPLETE)
            {
                PrintLog("\n #Error: reflection FRAMEBUFFER IS NOT COMPLETE . ");
                return false;
            }
            glBindRenderbuffer(GL_RENDERBUFFER,0);	
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        //# Step 3 :

        return TRUE;
    }

    void renderSurfaceQuad()
    {
        //code:
        
        glUseProgram(shaderProgramObject_SurfaceQuad);


        glUniformMatrix4fv(modelMatrixUniform_SurfaceQuad, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewMatrixUniform_SurfaceQuad, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projectionMatrixUniform_SurfaceQuad, 1, GL_FALSE, perspectiveProjectionMatrix);

        //Following is MultiTexturing- which is base of many special effect

        glActiveTexture(GL_TEXTURE0);//binding Reflection Texture
        glUniform1i(reflectionTextureSampler_SurfaceQuad,0);//send sampler uniform 
        glBindTexture(GL_TEXTURE_2D,fbo_texture_reflection);//bind texture:

        glBindVertexArray(vao_SurfaceQuad);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);

        glUseProgram(0);

    }

    void uninitialize(void)
    {
        UninitializeShaders(shaderProgramObject_SurfaceQuad);
    }

};


