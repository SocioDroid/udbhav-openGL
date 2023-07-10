#pragma once
#include "../../utils/common.h"

class SpotLightShader
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
	// GLuint lightColorUniform;

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

	// for lights

	// GLfloat lightPosition[] = { 0.0f, 0.0f, 10.0f, 1.0f };

	BOOL bLight = FALSE;

	// For Sphere
	float sphere_vertices[1146];
	float sphere_normals[1146];
	float sphere_textures[764];
	unsigned short sphere_elements[2280];

	int numVertices;
	int numElements;

	float angleRotation_Deg = 0.0f;
	float radiusForLightRotation = 8.0f;

	/*
	float constantAttenuation = 1.0f;
	float linearAttenuation = 0.0f;
	float quadraticAttenuation = 0.0f;
	*/

	/*
	float constantAttenuation = 1.0f;
	float linearAttenuation = 0.5f;
	float quadraticAttenuation = 0.0f;
	*/

	float constantAttenuation = 1.0f;
	float linearAttenuation = 0.0f;
	float quadraticAttenuation = 0.0f;

	BOOL gbLeftArrowIsPressed = FALSE;
	BOOL gbRightArrowIsPressed = FALSE;
	BOOL gbUpArrowIsPressed = FALSE;
	BOOL gbDownArrowIsPressed = FALSE;

	float gbYAxisChange = 0.0f;

	// For Square
	GLuint vaoSquare;
	GLuint vbo_Square_position;
	GLuint vbo_Square_normal;

	// Member Functions
	BOOL initialize()
	{
		// ****** SHADERS ******
		GLuint vertexShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\SpotLight\\SpotLight.vs", VERTEX);
		if (vertexShaderObject == 0)
		{
			PrintLog("CreateAndCompileShaderObjects() Failed For \"SpotLight.vs\" Shader File.\n");
			return (FALSE);
		}

		GLuint fragmentShaderObject = CreateAndCompileShaderObjects(".\\src\\shaders\\SpotLight\\SpotLight.fs", FRAGMENT);
		if (fragmentShaderObject == 0)
		{
			PrintLog("CreateAndCompileShaderObjects() Failed For \"SpotLight.fs\" Shader File.\n");
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

		// ================================== Geometry
		//------------------- Sphere
		/*
			getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
			numVertices = getNumberOfSphereVertices();
			numElements = getNumberOfSphereElements();

			// VAO-VBO Related Code
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			// VBO For Position
			glGenBuffers(1, &vbo_position);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_position);

			glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			//vbo Normals
			glGenBuffers(1, &vbo_normals);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);

			glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
			glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			//vbo Elements
			glGenBuffers(1, &vbo_elements);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elements);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glBindVertexArray(0);

			//------------------- Square
			const GLfloat squarePosition[] =
			{
				1.0f, 1.0f, 0.0f, //right upper
				-1.0f, 1.0f, 0.0f,//left upper
				-1.0f, -1.0f, 0.0f,//left lower
				1.0f, -1.0f, 0.0f //right lower
			};

			const GLfloat squareNormal[] =
			{
				0.0f, 0.0f, 1.0f, //right upper
				0.0f, 0.0f, 1.0f,//left upper
				0.0f, 0.0f, 1.0f,//left lower
				0.0f, 0.0f, 1.0f //right lower
			};

			//vaoSquare related code
			//=======================

			glGenVertexArrays(1, &vaoSquare);
			glBindVertexArray(vaoSquare);

			//vbo - Triangle - Position
			glGenBuffers(1, &vbo_Square_position);//&vbo : gives GPU's memory address.
			glBindBuffer(GL_ARRAY_BUFFER, vbo_Square_position);// 1 : Target, 2 : what to be bound with the target? - vbo

			glBufferData(GL_ARRAY_BUFFER, sizeof(squarePosition), squarePosition, GL_STATIC_DRAW);

			glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			//this function enables us to visualize it
			//1: where to look for it and map to what, 2: set of member count (3), 3: type of those members, 4:
			glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			//vbo - Triangle - Position
			glGenBuffers(1, &vbo_Square_normal);//&vbo : gives GPU's memory address.
			glBindBuffer(GL_ARRAY_BUFFER, vbo_Square_normal);// 1 : Target, 2 : what to be bound with the target? - vbo

			glBufferData(GL_ARRAY_BUFFER, sizeof(squareNormal), squareNormal, GL_STATIC_DRAW);

			glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			//this function enables us to visualize it
			//1: where to look for it and map to what, 2: set of member count (3), 3: type of those members, 4:
			glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(0);//Unbind vao Square
		*/
		return TRUE;
	}

	void uninitialize(void)
	{
		UninitializeShaders(shaderProgramObject);
	}
};
