#pragma once

#include "../../utils/common.h"
#include "../../utils/camera/BezierCamera.h"
#include "WaterQuad_Shader.h"

using namespace vmath;

#define REFLECTION_FBO_WIDTH 1920
#define REFLECTION_FBO_HEIGHT 1080

#define REFRACTION_FBO_WIDTH 1920
#define REFRACTION_FBO_HEIGHT 1080

#define WATER_WAVE_SPEED 0.002f

#define VIGNETTE_OUTER_RADIUS 1.0f
#define VIGNETTE_INNER_RADIUS 0.8f

#define WATER_QUAD_SIZE 1000. * 90.

// ==================== Extern Variables

extern Camera camera;
extern BezierCamera *globalBezierCamera;

class WaterMatrix
{
public:
	// FBO-related variables
	GLuint fbo_reflection;
	GLuint rbo_reflection;
	GLuint fbo_texture_reflection;

	GLuint fbo_refraction;
	GLuint rbo_texture_refraction;
	GLuint fbo_texture_refraction;

	// For Vignette
	GLfloat gfVignetteOuterRadius = VIGNETTE_OUTER_RADIUS;
	GLfloat gfVignetteInnerRadius = VIGNETTE_INNER_RADIUS;

	// Water
	GLuint vao_waterQuad;
	GLuint vbo_waterQuad_position;

	GLuint vao_waterBedQuad;
	GLuint vbo_waterBedQuad_position;

	GLuint texture_waterBedFloor = 0;
	GLuint textureSamplerUniform_waterBedQuad;
	GLuint texture_waterDUDVMap = 0;
	GLuint texture_waterNormalMap = 0;

	GLfloat moveFactor = 0.0f;

	WaterQuadShader waterQuadShader;
	// WaterBedQuadShader waterBedQuadShader;
	float waterHeight = 0.0f;
	float interpolateWaterColor = 0.0f;

	vec3 camPosition;
	vec3 camCenter;
	vec3 camUp;

	bool isInitialized = false;
	WaterMatrix()
	{
	}

	int initializeWaterQuad(void)
	{
		// VERTICES
		const GLfloat waterQuadPositions[] =
			{
				-WATER_QUAD_SIZE, -WATER_QUAD_SIZE,
				-WATER_QUAD_SIZE, WATER_QUAD_SIZE,
				WATER_QUAD_SIZE, -WATER_QUAD_SIZE,

				WATER_QUAD_SIZE, -WATER_QUAD_SIZE,
				-WATER_QUAD_SIZE, WATER_QUAD_SIZE,
				WATER_QUAD_SIZE, WATER_QUAD_SIZE};

		// VAO-VBO Related Code
		glGenVertexArrays(1, &vao_waterQuad);
		glBindVertexArray(vao_waterQuad);

		// VBO For Position
		glGenBuffers(1, &vbo_waterQuad_position);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_waterQuad_position);

		glBufferData(GL_ARRAY_BUFFER, sizeof(waterQuadPositions), waterQuadPositions, GL_STATIC_DRAW);
		glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		return (0);
	}

	int initializeWaterBedQuad(void)
	{
		// VERTICES
		const GLfloat waterBedQuadPositions[] =
			{
				-350.0f, -350.0f,
				-350.0f, 350.0f,
				350.0f, -350.0f,

				350.0f, -350.0f,
				-350.0f, 350.0f,
				350.0f, 350.0f};

		// VAO-VBO Related Code
		glGenVertexArrays(1, &vao_waterBedQuad);
		glBindVertexArray(vao_waterBedQuad);

		// VBO For Position
		glGenBuffers(1, &vbo_waterBedQuad_position);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_waterBedQuad_position);

		glBufferData(GL_ARRAY_BUFFER, sizeof(waterBedQuadPositions), waterBedQuadPositions, GL_STATIC_DRAW);
		glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		return (0);
	}

	bool initializeReflectionFBO(GLint reflectionTextureWidth, GLint reflectionTextureHeight)
	{
		// variables
		int maxRenderbufferSize;

		// code
		// 1: Check Available renderbuffer size
		glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
		if (maxRenderbufferSize < reflectionTextureWidth || maxRenderbufferSize < reflectionTextureHeight)
		{
			PrintLog("Reflection : Insufficient Renderbuffer size\n");
			return (false);
		}

		// 2: Create Framebuffer object
		glGenFramebuffers(1, &fbo_reflection);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo_reflection);

		// 3: Create Renderbuffer object
		glGenRenderbuffers(1, &rbo_reflection);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo_reflection);

		// 4: Where to keep this Renderbuffer and what will be the format of Renderbuffer
		// i.e. storage and format of Renderbuffer
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, reflectionTextureWidth, reflectionTextureHeight); // Target, format, width, height

		// 5: Create Empty texture to be filled with scene later
		glGenTextures(1, &fbo_texture_reflection);
		glBindTexture(GL_TEXTURE_2D, fbo_texture_reflection);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, reflectionTextureWidth, reflectionTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		// 6: Give above texture to fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture_reflection, 0);

		// 7: Give renderbuffer to fbo
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_reflection);

		// Check whether the framebuffer is created successfully or not
		GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (result != GL_FRAMEBUFFER_COMPLETE)
		{
			PrintLog("Reflection Framebuffer is not complete\n");
			return (false);
		}

		// unbind Framebuffer, when it is done, renderbuffer also gets automatically unbound.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return (true);
	}

	bool initializeRefractionFBO(GLint refractionTextureWidth, GLint refractionTextureHeight)
	{
		// variables
		int maxRenderbufferSize;

		// code
		// 1: Check Available renderbuffer size
		glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
		if (maxRenderbufferSize < refractionTextureWidth || maxRenderbufferSize < refractionTextureHeight)
		{
			PrintLog("Refraction : Insufficient Renderbuffer size\n");
			return (false);
		}

		// 2: Create Framebuffer object
		glGenFramebuffers(1, &fbo_refraction);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo_refraction);

		// 3: Create Empty texture to be filled with scene later
		glGenTextures(1, &fbo_texture_refraction);
		glBindTexture(GL_TEXTURE_2D, fbo_texture_refraction);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, refractionTextureWidth, refractionTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		// 4: Give above texture to fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture_refraction, 0);

		// 5: Create Empty texture to be filled with scene later
		glGenTextures(1, &rbo_texture_refraction);
		glBindTexture(GL_TEXTURE_2D, rbo_texture_refraction);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, refractionTextureWidth, refractionTextureHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		// depth data is in float

		// 6: Give above texture to fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rbo_texture_refraction, 0);

		// Check whether the framebuffer is created successfully or not
		GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (result != GL_FRAMEBUFFER_COMPLETE)
		{
			PrintLog("Refraction Framebuffer is not complete\n");
			return (false);
		}

		// unbind Framebuffer, when it is done, renderbuffer also gets automatically unbound.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return (true);
	}

	int initialize(void)
	{
		waterQuadShader.initialize();
		// waterBedQuadShader.initialize();
		int iRetVal = -1;

		iRetVal = initializeWaterQuad();
		if (iRetVal != 0)
		{
			PrintLog("initializeWaterQuad() Failed.\n ");
			return (iRetVal);
		}

		iRetVal = initializeWaterBedQuad();
		if (iRetVal != 0)
		{
			PrintLog("initializeWaterBedQuad() Failed.\n ");
			return (iRetVal);
		}

		//-------------------------------------
		if (initializeReflectionFBO(REFLECTION_FBO_WIDTH, REFLECTION_FBO_HEIGHT) == false)
		{
			PrintLog("initializeReflectionFBO() Failed.\n ");
			return (-6);
		}

		if (initializeRefractionFBO(REFRACTION_FBO_WIDTH, REFRACTION_FBO_HEIGHT) == false)
		{
			PrintLog("initializeRefractionFBO() Failed.\n ");
			return (-7);
		}

		// //-------------------------------------
		if (LoadPNGImage(&texture_waterDUDVMap, ".\\assets\\textures\\water\\waterDUDV.png") == FALSE)
		{
			PrintLog("LoadPNGImage Failed for waterDUDV\n");
			return (-8);
		}

		if (LoadPNGImage(&texture_waterNormalMap, ".\\assets\\textures\\water\\waterNormalMap.png") == FALSE)
		{
			PrintLog("LoadPNGImage Failed for waterNormalMap\n");
			return (-9);
		}

		// initialize_JisDesh();
		isInitialized = true;
		return (0);
	}

	void bindReflectionFBO(GLint textureWidth, GLint textureHeight)
	{
		// code
		glBindFramebuffer(GL_FRAMEBUFFER, fbo_reflection);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, textureWidth, textureHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)textureWidth / (GLfloat)textureHeight, 0.1f, 1000000.0f);

		if (USE_FPV_CAM)
		{
			camera.invertPitch();
			camera.position[1] -= 2 * (camera.position[1] - waterHeight);
			viewMatrix = camera.getViewMatrix();
		}
		else
		{
			globalBezierCamera->invertPitch();
			globalBezierCamera->position[1] -= 2 * (globalBezierCamera->position[1] - waterHeight);
			viewMatrix = globalBezierCamera->getViewMatrix();
		}
	}

	void unbindReflectionFBO()
	{
		if (USE_FPV_CAM)
		{
			camera.invertPitch();
			camera.position[1] += 2 * abs(camera.position[1] - waterHeight);
			viewMatrix = camera.getViewMatrix();
		}
		else
		{
			globalBezierCamera->invertPitch();
			globalBezierCamera->position[1] -= 2 * (globalBezierCamera->position[1] - waterHeight);
			viewMatrix = globalBezierCamera->getViewMatrix();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void bindRefractionFBO(GLint textureWidth, GLint textureHeight)
	{
		// code
		glBindFramebuffer(GL_FRAMEBUFFER, fbo_refraction);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, textureWidth, textureHeight);

		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)textureWidth / (GLfloat)textureHeight, 0.1f, 10000000.0f);

		if (USE_FPV_CAM)
		{
			viewMatrix = camera.getViewMatrix();
		}
		else
		{
			viewMatrix = globalBezierCamera->getViewMatrix();
		}
	}

	void unbindRefractionFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void renderWaterQuad(float wh)
	{
		waterHeight = wh;
		// variable declarations
		float lightColor[] = {1.0f, 1.0f, 1.0f}; // white light
		float lightPosition[] = {0.0f, 100.0f, 10.0f};
		// float lightPosition[] = {0.0f, 100.0f, 0.0f};

		// code
		pushMatrix(modelMatrix);
		{
			glUseProgram(waterQuadShader.shaderProgramObject_waterQuad);

			glUniformMatrix4fv(waterQuadShader.modelMatrixUniform_waterQuad, 1, GL_FALSE, modelMatrix);
			glUniformMatrix4fv(waterQuadShader.viewMatrixUniform_waterQuad, 1, GL_FALSE, viewMatrix);
			glUniformMatrix4fv(waterQuadShader.projectionMatrixUniform_waterQuad, 1, GL_FALSE, perspectiveProjectionMatrix);

			glUniform3fv(waterQuadShader.cameraPositionUniform_waterQuad, 1, (USE_FPV_CAM ? camera.getEye() : globalBezierCamera->getEye()));

			glUniform3fv(waterQuadShader.lightPositionUniform_waterQuad, 1, lightPosition);
			glUniform3fv(waterQuadShader.lightColorUniform_waterQuad, 1, lightColor);

			moveFactor = moveFactor + WATER_WAVE_SPEED;
			glUniform1f(waterQuadShader.moveFactorOffsetUniform_waterQuad, moveFactor);

			glUniform1f(waterQuadShader.interpolateDarkToBright_uniform, interpolateWaterColor);

			// For Vignette
			glUniform2f(waterQuadShader.resolutionUniform, (GLfloat)1920, (GLfloat)1080);

			if (gfVignetteOuterRadius < 1.0f)
			{
				gfVignetteOuterRadius = gfVignetteOuterRadius + 0.002f;
				glUniform1f(waterQuadShader.vignetteOuterRadiusUniform, gfVignetteOuterRadius);
			}

			if (gfVignetteInnerRadius < 0.8f)
			{
				gfVignetteInnerRadius = gfVignetteInnerRadius + 0.001f;
				glUniform1f(waterQuadShader.vignetteInnerRadiusUniform, gfVignetteInnerRadius);
			}

			// Following is multitexturing
			// Bind reflection texture
			glActiveTexture(GL_TEXTURE0);
			glUniform1i(waterQuadShader.reflectionTextureSamplerUniform_waterQuad, 0);
			glBindTexture(GL_TEXTURE_2D, fbo_texture_reflection);

			// Bind refraction texture
			glActiveTexture(GL_TEXTURE1);
			glUniform1i(waterQuadShader.refractionTextureSamplerUniform_waterQuad, 1);
			glBindTexture(GL_TEXTURE_2D, fbo_texture_refraction);

			// Bind Water DUDVMap texture
			glActiveTexture(GL_TEXTURE2);
			glUniform1i(waterQuadShader.waterDUDVMapTextureSamplerUniform_waterQuad, 2);
			glBindTexture(GL_TEXTURE_2D, texture_waterDUDVMap);

			// Bind Water DUDVMap texture
			glActiveTexture(GL_TEXTURE3);
			glUniform1i(waterQuadShader.waterNormalMapTextureSamplerUniform_waterQuad, 3);
			glBindTexture(GL_TEXTURE_2D, texture_waterNormalMap);

			glBindVertexArray(vao_waterQuad);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glUseProgram(0);
		}
		modelMatrix = popMatrix();
	}

	void uninitialize_WaterMatrix(void)
	{
		if (isInitialized)
		{

			if (texture_waterNormalMap)
			{
				glDeleteTextures(1, &texture_waterNormalMap);
				texture_waterNormalMap = 0;
			}

			if (texture_waterDUDVMap)
			{
				glDeleteTextures(1, &texture_waterDUDVMap);
				texture_waterDUDVMap = 0;
			}

			uninitializeRefractionFBO();
			uninitializeReflectionFBO();

			// ****** SHADER-RELATED UNINITIALIZATIONS ******
			UninitializeShaders(waterQuadShader.shaderProgramObject_waterQuad);
			// UninitializeShaders(waterBedQuadShader.shaderProgramObject_waterBedQuad);

			if (vbo_waterBedQuad_position)
			{
				glDeleteBuffers(1, &vbo_waterBedQuad_position);
				vbo_waterBedQuad_position = 0;
			}

			if (vao_waterBedQuad)
			{
				glDeleteVertexArrays(1, &vao_waterBedQuad);
				vao_waterBedQuad = 0;
			}

			if (vbo_waterQuad_position)
			{
				glDeleteBuffers(1, &vbo_waterQuad_position);
				vbo_waterQuad_position = 0;
			}

			if (vao_waterQuad)
			{
				glDeleteVertexArrays(1, &vao_waterQuad);
				vao_waterQuad = 0;
			}
		}
	}

	void uninitializeReflectionFBO(void)
	{
		//
		if (fbo_texture_reflection)
		{
			glDeleteTextures(1, &fbo_texture_reflection);
			fbo_texture_reflection = 0;
		}

		if (rbo_reflection)
		{
			glDeleteRenderbuffers(1, &rbo_reflection);
			rbo_reflection = 0;
		}

		if (fbo_reflection)
		{
			glDeleteFramebuffers(1, &fbo_reflection);
			fbo_reflection = 0;
		}
	}

	void uninitializeRefractionFBO(void)
	{
		if (rbo_texture_refraction)
		{
			glDeleteTextures(1, &rbo_texture_refraction);
			rbo_texture_refraction = 0;
		}

		if (fbo_texture_refraction)
		{
			glDeleteTextures(1, &fbo_texture_refraction);
			fbo_texture_refraction = 0;
		}

		if (fbo_refraction)
		{
			glDeleteFramebuffers(1, &fbo_refraction);
			fbo_refraction = 0;
		}
	}
};
