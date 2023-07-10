#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 36) out;

// OUT
out vec2 texCoord;
out float colorVariation;

// UNIFORMS
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform vec3 u_cameraPosition;
uniform sampler2D u_wind;
uniform float u_time;
uniform float u_windStrength;

// Variable Declarations
float grassSize;

// Constants
#define c_min_size 0.4f
#define LOD1 500.0f
#define LOD2 1000.0f
#define LOD3 1500.0f
#define PI 3.141592653589793
#define NUM_OCTAVES 5

// Utility Functions
//-----------------------------------------------------------------------

mat4 rotationX(in float angle)
{
	return mat4(	1.0,  0,           0,             0,
					0,	  cos(angle),  -sin(angle),   0,
					0,    sin(angle),  cos(angle),    0,
					0,    0,           0,             1);
}

mat4 rotationY(in float angle)
{
	return mat4(	cos(angle),  	0,          sin(angle),     0,
					0,	  			1.0,  		0,   			0,
					-sin(angle),    0,  		cos(angle),    	0,
					0,    			0,          0,             	1);
}

mat4 rotationZ(in float angle)
{
	return mat4(	cos(angle),  	-sin(angle),	0,	 0,
					sin(angle),	  	cos(angle),  	0,   0,
					0,    			0,  			1,   0,
					0,    			0,          	0,   1);
}

float random(vec2 st)
{
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float noise(in vec2 st)
{
	vec2 i = floor(st);
	vec2 f = fract(st);

	// 4 corners in 2D of a tile
	float a = random(i);
	float b = random(i + vec2(1.0, 0.0));
	float c = random(i + vec2(0.0, 1.0));
	float d = random(i + vec2(1.0, 1.0));

	// Cubic Hermine Curve
	vec2 u = f * f * (3.0 - 2.0 * f);

	// Mix 4 corners' Percentages
	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

float fbm(in vec2 st)
{
	float v = 0.0;
	float a = 0.5;

	vec2 shift = vec2(100.0);

	// Rotate To Reduce Axial Bias
	mat2 rotate = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.5));

	for (int i = 0; i < NUM_OCTAVES; i++)
	{
		v = v + a * noise(st);
		st = rotate * st * 2.0 + shift;
		a = a * 0.5;
	}

	return v;
}

//-----------------------------------------------------------------------

void createQuad(vec3 basePosition, mat4 crossModel)
{
	// Variable Declarations
	vec4 vertices[4];
	vec2 texcoords[4];
	vec2 windDirection = vec2(1.0, 1.0);
	mat4 modelWindMatrix = mat4(1);

	// Code
	vertices[0] = vec4(-0.25, 0.0, 0.0, 0.0);	// Bottom Left
	vertices[1] = vec4(0.25, 0.0, 0.0, 0.0);	// Bottom Right
	vertices[2] = vec4(-0.25, 0.5, 0.0, 0.0);	// Top Left
	vertices[3] = vec4(0.25, 0.5, 0.0, 0.0);	// Top Right

	texcoords[0] = vec2(0.0, 0.0);	// Bottom Left
	texcoords[1] = vec2(1.0, 0.0);	// Bottom Right
	texcoords[2] = vec2(0.0, 1.0);	// Top Left
	texcoords[3] = vec2(1.0, 1.0);	// Top Right

	// Wind
	vec2 uv = (basePosition.xz / 10.0) + windDirection * u_windStrength * u_time;
	uv.x = mod(uv.x, 1.0);
	uv.y = mod(uv.y, 1.0);

	vec4 wind = texture(u_wind, uv);
	mat4 modelWind = rotationX(wind.x * PI * 0.75f - PI * 0.25f) * rotationZ(wind.y * PI * 0.75f - PI * 0.25f);
	
	// Random Y-Axis Rotation
	mat4 modelRandomYRotation = rotationY(random(basePosition.xz) * PI);

	// Billboard Creation Loop
	for (int i = 0; i < 4; i++)
	{
		if (i == 2)
			modelWindMatrix = modelWind;
		
		gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * (gl_in[0].gl_Position + modelWindMatrix * modelRandomYRotation * crossModel * (vertices[i] * grassSize));

		texCoord = texcoords[i];
		colorVariation = fbm(gl_in[0].gl_Position.xz);

		EmitVertex();
	}

	EndPrimitive();
}

void createGrass(int numberOfQuads)
{
	// Variable Declarations
	mat4 model0 = mat4(1.0);
	mat4 model45 = rotationY(radians(45));
	mat4 modelm45 = rotationY(-radians(45));

	switch(numberOfQuads)
	{
		case 1:
			createQuad(gl_in[0].gl_Position.xyz, model0);
		break;

		case 2:
			createQuad(gl_in[0].gl_Position.xyz, model45);
			createQuad(gl_in[0].gl_Position.xyz, modelm45);
		break;

		case 3:
			createQuad(gl_in[0].gl_Position.xyz, model0);
			createQuad(gl_in[0].gl_Position.xyz, model45);
			createQuad(gl_in[0].gl_Position.xyz, modelm45);
		break;
	}
}

void main()
{
	// Variable Declarations
	vec3 cameraDistance = gl_in[0].gl_Position.xyz - u_cameraPosition;
	float distanceLength = length(cameraDistance);
	float transitionFactor = 6.0;
	int details = 3;

	// Random Grass Size
	grassSize = random(gl_in[0].gl_Position.xz) * (1.0 - c_min_size) + c_min_size;
	
	// Distance of the Camera's position with transition
	if (distanceLength > LOD2)
	{
		transitionFactor = transitionFactor * 1.5f;
	}
	distanceLength = distanceLength + (random(gl_in[0].gl_Position.xz) * transitionFactor - transitionFactor / 2.0f);

	// Changing details depending on length
	if (distanceLength > LOD1)
		details = 1;
	if (distanceLength > LOD2)
		details = 1;
	if (distanceLength > LOD3)
		details = 0;
	
	// Grass Creation
	if (details != 1 
		|| (details == 1 && (int(gl_in[0].gl_Position.x * 10) % 1) == 0 || (int(gl_in[0].gl_Position.z * 10) % 1) == 0)
		|| (details == 2 && (int(gl_in[0].gl_Position.x * 5) % 1) == 0 || (int(gl_in[0].gl_Position.z * 5) % 1) == 0))
	{
		createGrass(details);
	}
} 

