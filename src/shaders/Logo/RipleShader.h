#pragma once
#include "../../utils/common.h"

class RipleShader
{
public:
	// Variables

	GLuint shaderProgramObject;

	GLuint modelMatrixUniform;
	GLuint viewMatrixUniform;
	GLuint projectionMatrixUniform;

	GLuint u_offestUniform;
	GLuint u_scaleInUniform;
	GLuint u_scaleOutUniform;

	// SpotLight
	GLuint laUniform[4];
	GLuint ldUniform[4];
	GLuint lsUniform[4];
	GLuint lightPositionUniform[4];

	GLuint kaUniform;
	GLuint kdUniform;
	GLuint ksUniform;
	GLuint shininessUniform;
	GLuint strengthUniform;

	GLuint eyeDirectionUniform;
	GLuint constantAttenuationUniform;
	GLuint linearAttenuationUniform;
	GLuint quadraticAttenuationUniform;

	GLuint coneDirectionUniform;
	GLuint spotCosCutoffUniform;
	GLuint spotExponentUniform;

	GLuint lightingEnabledUniform;

	float angleRotation_Deg = 0.0f;
	float radiusForLightRotation = 8.0f;

	float constantAttenuation = 1.0f;
	float linearAttenuation = 0.0f;
	float quadraticAttenuation = 0.0f;

	BOOL gbLeftArrowIsPressed = FALSE;
	BOOL gbRightArrowIsPressed = FALSE;
	BOOL gbUpArrowIsPressed = FALSE;
	BOOL gbDownArrowIsPressed = FALSE;

	BOOL bLight = FALSE;

	// Member Functions
	BOOL initialize()
	{
		// ****** SHADERS ******
		GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\Logo\\Riple.vs", VERTEX);
		if (vertexShaderObject == 0)
		{
			PrintLog("CreateAndCompileShaderObjects() Failed For Riple VS.\n");
			return (FALSE);
		}

		GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\Logo\\Riple.fs", FRAGMENT);
		if (fragmentShaderObject == 0)
		{
			PrintLog("CreateAndCompileShaderObjects() Failed For Riple FS.\n");
			return (FALSE);
		}

		shaderProgramObject = glCreateProgram();

		glAttachShader(shaderProgramObject, vertexShaderObject);
		glAttachShader(shaderProgramObject, fragmentShaderObject);

		glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_POSITION, "a_position");
		glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_NORMAL, "a_normal");
		glBindAttribLocation(shaderProgramObject, MATRIX_ATTRIBUTE_TEXTURE0, "a_texcoord");

		if (LinkShaderProgramObject(shaderProgramObject) == FALSE)
		{
			PrintLog("LinkShaderProgramObject() Failed.\n");
			return (FALSE);
		}

		// Post-Linking Getting Of Uniform Locations
		modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");
		viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
		projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");

		u_offestUniform = glGetUniformLocation(shaderProgramObject, "u_Offset");
		u_scaleInUniform = glGetUniformLocation(shaderProgramObject, "u_scaleIn");
		u_scaleOutUniform = glGetUniformLocation(shaderProgramObject, "u_scaleOut");
		
		laUniform[0] = glGetUniformLocation(shaderProgramObject, "u_la[0]");
		ldUniform[0] = glGetUniformLocation(shaderProgramObject, "u_ld[0]");
		lsUniform[0] = glGetUniformLocation(shaderProgramObject, "u_ls[0]");
		lightPositionUniform[0] = glGetUniformLocation(shaderProgramObject, "u_lightPosition[0]");

		laUniform[1] = glGetUniformLocation(shaderProgramObject, "u_la[1]");
		ldUniform[1] = glGetUniformLocation(shaderProgramObject, "u_ld[1]");
		lsUniform[1] = glGetUniformLocation(shaderProgramObject, "u_ls[1]");
		lightPositionUniform[1] = glGetUniformLocation(shaderProgramObject, "u_lightPosition[1]");

		laUniform[2] = glGetUniformLocation(shaderProgramObject, "u_la[2]");
		ldUniform[2] = glGetUniformLocation(shaderProgramObject, "u_ld[2]");
		lsUniform[2] = glGetUniformLocation(shaderProgramObject, "u_ls[2]");
		lightPositionUniform[2] = glGetUniformLocation(shaderProgramObject, "u_lightPosition[2]");

		laUniform[3] = glGetUniformLocation(shaderProgramObject, "u_la[3]");
		ldUniform[3] = glGetUniformLocation(shaderProgramObject, "u_ld[3]");
		lsUniform[3] = glGetUniformLocation(shaderProgramObject, "u_ls[3]");
		lightPositionUniform[3] = glGetUniformLocation(shaderProgramObject, "u_lightPosition[3]");

		kaUniform = glGetUniformLocation(shaderProgramObject, "u_ka");
		kdUniform = glGetUniformLocation(shaderProgramObject, "u_kd");
		ksUniform = glGetUniformLocation(shaderProgramObject, "u_ks");
		shininessUniform = glGetUniformLocation(shaderProgramObject, "u_shininess");
		strengthUniform = glGetUniformLocation(shaderProgramObject, "u_strength");

		eyeDirectionUniform = glGetUniformLocation(shaderProgramObject, "u_eyeDirection");
		constantAttenuationUniform = glGetUniformLocation(shaderProgramObject, "u_constantAttenuation");
		linearAttenuationUniform = glGetUniformLocation(shaderProgramObject, "u_linearAttenuation");
		quadraticAttenuationUniform = glGetUniformLocation(shaderProgramObject, "u_quadraticAttenuation");

		coneDirectionUniform = glGetUniformLocation(shaderProgramObject, "u_coneDirection");
		spotCosCutoffUniform = glGetUniformLocation(shaderProgramObject, "u_spotCosCutoff");
		spotExponentUniform = glGetUniformLocation(shaderProgramObject, "u_spotExponent");

		lightingEnabledUniform = glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");

		return TRUE;
	}

	

	void uninitialize(void)
	{
		UninitializeShaders(shaderProgramObject);
	}
};
