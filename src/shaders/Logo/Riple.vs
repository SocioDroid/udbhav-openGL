#version 460 core    

in vec4 a_position;    
in vec3 a_normal;
in vec2 a_texcoord; 

uniform mat4 u_modelMatrix;    
uniform mat4 u_viewMatrix;    
uniform mat4 u_projectionMatrix;    

uniform vec3 u_Offset;    
uniform float u_scaleIn;    
uniform float u_scaleOut;    

uniform int u_lightingEnabled;

out vec3 a_normal_out;
out vec4 a_position_out;
out vec2 a_texcoord_out;  

float mod289(float x)    
{return x - floor(x * (1.0 / 289.0)) * 289.0;}    

vec4 mod289(vec4 x)    
{return x - floor(x * (1.0 / 289.0)) * 289.0;}    

vec4 perm(vec4 x)    
{return mod289(((x * 34.0) + 1.0) * x);}    


float noise31(vec3 p)    
{    
vec3 a = floor(p);    
vec3 d = p - a;    
d = d * d * (3.0 - 2.0 * d);    

vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);    
vec4 k1 = perm(b.xyxy);    
vec4 k2 = perm(k1.xyxy + b.zzww);    

vec4 c = k2 + a.zzzz;    
vec4 k3 = perm(c);    
vec4 k4 = perm(c + 1.0);    

vec4 o1 = fract(k3 * (1.0 / 41.0));    
vec4 o2 = fract(k4 * (1.0 / 41.0));    

vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);    
vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);    

return o4.y * d.y + o4.x * (1.0 - d.y);    
}    

void main(void)    
{    

    if(u_lightingEnabled == 1)
	{
		mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);
		a_normal_out = normalize(normalMatrix * a_normal);
		a_position_out = u_viewMatrix * u_modelMatrix * a_position;
	}
    a_texcoord_out = a_texcoord;

    vec3 vertex = a_position.xyz + vec3(noise31(u_Offset + a_position.xyz * u_scaleIn)) * u_scaleOut;  

    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position.x,vertex.y,vertex.z,1.0);    
};

