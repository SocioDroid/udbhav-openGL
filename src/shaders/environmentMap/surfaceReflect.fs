//surfaceReflect.FS

#version 460 core

//IN
in vec2 a_texcoords_out;
in vec4 clipSpaceCoords;

//UNIFORM
uniform sampler2D u_reflectionTextureSampler;

//OUT

out vec4 FragColor;

void main(void)
{
	//code:
	
	//1:convert clipSpace coordinates to Screen-Space coordinates:
	vec2 screenSpaceCoords;
	
	screenSpaceCoords.x=clipSpaceCoords.x/clipSpaceCoords.w;
	screenSpaceCoords.y=clipSpaceCoords.y/clipSpaceCoords.w;

	screenSpaceCoords.y=(-1.0)*screenSpaceCoords.y;
	screenSpaceCoords.x=(-1.0)*screenSpaceCoords.x;

	//2: convert screen-space coordinates to Normalized Device Co-Ordinates:
	vec2 ndcCoords;
	ndcCoords.x=screenSpaceCoords.x/2.0+0.5;
	ndcCoords.y=screenSpaceCoords.y/2.0+0.5;

	//3: Reflection Texture

	vec2 reflectTexcoords=vec2(ndcCoords.x,-ndcCoords.y);
	//reflectTexcoords=reflectTexcoords

	//Clamp it between 0.001 and 0.999
	reflectTexcoords.x=clamp(reflectTexcoords.x,0.001,0.999);
	reflectTexcoords.y=clamp(reflectTexcoords.y,-0.999,-0.001);

	vec4 reflectColor=texture(u_reflectionTextureSampler,reflectTexcoords);

	vec4 color=reflectColor;

	FragColor=mix(color,vec4(1.0,1.0,1.0,1.0),0.4);
	// FragColor=vec4(1.0,0.0,0.0,1.0);
}

