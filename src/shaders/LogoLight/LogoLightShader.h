#pragma once
#include "../../utils/common.h"

class LogoLightShader
{
public:
	// Variables

	GLuint shaderProgramObject;

	GLuint vao;
	GLuint vbo_position;
	GLuint vbo_normals;
	GLuint vbo_elements;

	GLuint modelMatrixUniform;
	GLuint viewMatrixUniform;
	GLuint projectionMatrixUniform;

	GLuint laUniform;
	GLuint ldUniform;
	GLuint lsUniform;
	GLuint lightPositionUniform;

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

	BOOL bLight = FALSE;

	float angleRotation_Deg = 0.0f;
	float radiusForLightRotation = 8.0f;

	float constantAttenuation = 1.0f;
	float linearAttenuation = 0.0f;
	float quadraticAttenuation = 0.0f;

	BOOL gbLeftArrowIsPressed = FALSE;
	BOOL gbRightArrowIsPressed = FALSE;
	BOOL gbUpArrowIsPressed = FALSE;
	BOOL gbDownArrowIsPressed = FALSE;

	// Member Functions
	BOOL initialize()
	{
		// ****** SHADERS ******
		GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\LogoLight\\LogoLight.vs", VERTEX);
		if (vertexShaderObject == 0)
		{
			PrintLog("CreateAndCompileShaderObjects() Failed For \"LogoLight.vs\" Shader File.\n");
			return (FALSE);
		}

		GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\LogoLight\\LogoLight.fs", FRAGMENT);
		if (fragmentShaderObject == 0)
		{
			PrintLog("CreateAndCompileShaderObjects() Failed For \"LogoLight.fs\" Shader File.\n");
			return (FALSE);
		}

		shaderProgramObject = glCreateProgram();

		// Step 2
		glAttachShader(shaderProgramObject, vertexShaderObject);
		glAttachShader(shaderProgramObject, fragmentShaderObject);

		// Pre-Linking Binding Of Vertex Attribute Locations
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

		laUniform = glGetUniformLocation(shaderProgramObject, "u_la");
		ldUniform = glGetUniformLocation(shaderProgramObject, "u_ld");
		lsUniform = glGetUniformLocation(shaderProgramObject, "u_ls");
		lightPositionUniform = glGetUniformLocation(shaderProgramObject, "u_lightPosition");

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

	}	

	void uninitialize(void)
	{
		UninitializeShaders(shaderProgramObject);
	}
};
