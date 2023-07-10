#version 460 core

// Below line is for texelFetchBuffer()
#extension GL_EXT_gpu_shader4 : require

in vec4 a_positionMass;
in vec4 a_previousPosition;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform samplerBuffer u_texPositionMass;
uniform samplerBuffer u_previousTexPositionMass;

uniform vec2 u_inverseClothSize;
uniform vec2 u_step;
uniform int u_texSizeX;
uniform int u_texSizeY;

uniform float u_deltaTime;

uniform float u_ksStruct;
uniform float u_kdStruct;
uniform float u_ksShear;
uniform float u_kdShear;
uniform float u_ksBend;
uniform float u_kdBend;

uniform float u_defaultDamping;

uniform vec3 u_gravity;

uniform int u_backWallCollisionDetectionEnable;

out vec4 a_positionMass_out;
out vec4 a_previousPosition_out;

// Resolve constraint in this space
const vec3 center1 = vec3(-1.0, 0, 0);
const float radius1 = 4.0;

const vec3 center2 = vec3(0, 0, 0);
const float radius2 = 4.0;

const vec3 center3 = vec3(1.0, 0, 0);
const float radius3 = 4.0;

ivec2 getNextNeighbour(int n, out float ks, out float kd)
{
	// structural springs (adjacent neighbours)
	if(n < 4)
	{
		ks = u_ksStruct;
		kd = u_kdStruct;
	}

	if(n == 0)
	{
		return(ivec2(1, 0));
	}

	if(n == 1)
	{
		return(ivec2(0, -1));
	}

	if(n == 2)
	{
		return(ivec2(-1, 0));
	}

	if(n == 3)
	{
		return(ivec2(0, 1));
	}

	// shear springs (diagonal neighbours)
	if(n < 8)
	{
		ks = u_ksShear;
		kd = u_kdShear;
	}

	if(n == 4)
	{
		return(ivec2(1, -1));
	}

	if(n == 5)
	{
		return(ivec2(-1, -1));
	}

	if(n == 6)
	{
		return(ivec2(-1, 1));
	}

	if(n == 7)
	{
		return(ivec2(1, 1));
	}

	// bend spring (adjacent neighbours 1 node away)
	if(n < 12)
	{
		ks = u_ksBend;
		kd = u_kdBend;
	}

	if(n == 8)
	{
		return(ivec2(2, 0));
	}

	if(n == 9)
	{
		return(ivec2(0, -2));
	}

	if(n == 10)
	{
		return(ivec2(-2, 0));
	}

	if(n == 11)
	{
		return(ivec2(0, 2));
	}
}

void sphereCollision(inout vec3 x, vec3 center, float r)
{
	// code
	vec3 delta = x - center;
	float distance = length(delta);
	if(distance < r)
	{
		x = center + delta * (r / distance);
	}
}

void main(void)
{
	// code
	float mass = a_positionMass.w;
	
	vec3 position = a_positionMass.xyz;
	vec3 oldPosition = a_previousPosition.xyz;

	vec3 velocity = (position - oldPosition) / u_deltaTime;

	int index = gl_VertexID;
	int ix = index % u_texSizeX;
	int iy = index / u_texSizeX;
/*
	if(index == 0 || index == (u_texSizeX - 1))
	{
		mass = 0.0;
	}
*/

	if(index == 0 || index == (u_texSizeX - 1) || index == (u_texSizeX - 1) / 2 || index == (u_texSizeX - 1) / 4 || index == 3 * (u_texSizeX - 1) / 4)
	{
		mass = 0.0;
	}

	vec3 windForce = vec3(0.0, 0.0, -2.0);
	vec3 force = u_gravity * mass + windForce * (u_defaultDamping * velocity);

	float ks = 0.0, kd = 0.0;
	for(int k = 0; k < 12; k++)
	{
		ivec2 coord = getNextNeighbour(k, ks, kd);
		int j = coord.x;
		int i = coord.y;

		if(((iy + i) < 0) || ((iy + i) > (u_texSizeY - 1)))
		{
			continue;
		}

		if(((ix + j) < 0) || ((ix + j) > (u_texSizeX - 1)))
		{
			continue;
		}

		int index_neighbour = (iy + i) * u_texSizeX + ix + j;

		vec3 position2 = texelFetchBuffer(u_texPositionMass, index_neighbour).xyz;
		vec3 position2_previous = texelFetchBuffer(u_previousTexPositionMass, index_neighbour).xyz;

		vec2 coord_neighbour = vec2(ix + j, iy + i) * u_step;
		float rest_length = length(coord * u_inverseClothSize);

		vec3 velocity2 = (position2 - position2_previous) / u_deltaTime;

		vec3 deltaPosition = position - position2;
		vec3 deltaVelocity = velocity - velocity2;
		
		float distance = length(deltaPosition);
		float leftTerm = -ks * (distance - rest_length);
		float rightTerm = kd * (dot(deltaVelocity, deltaPosition) / distance);
		vec3 springForce = (leftTerm + rightTerm) * normalize(deltaPosition);
		force = force + springForce;
	}

	vec3 acceleration = vec3(0.0, 0.0, 0.0);
	if(mass != 0.0)
	{
		acceleration = force / mass;
	}

	vec3 temp = position;
	position = position * 2.0 - oldPosition + acceleration * u_deltaTime * u_deltaTime;
	oldPosition = temp;

	// collision with floor
	if(position.y < -2.0)
	{
		position.y = -2.0;
	}

	if(u_backWallCollisionDetectionEnable == 1)
	{
		// collision with back wall
		if(position.z < -1)
		{
			position.z = -1;
		}
	}

	a_positionMass_out = vec4(position, mass);
	a_previousPosition_out = vec4(oldPosition, mass);

	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(position, 1.0);

/*
	//Debug
	a_positionMass_out = vec4(ix, iy, 0.0, 1.0);
	a_previousPosition_out = a_positionMass_out;
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(position, 1.0);
*/
}






