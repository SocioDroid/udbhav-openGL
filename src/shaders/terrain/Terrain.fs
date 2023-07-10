#version 460 core

in vec2 a_texcoord_tes_out;
in vec3 FragPos_tes_out;
in vec3 TangentLightPos_tes_out;
in vec3 TangentViewPos_tes_out;
in vec3 TangentFragPos_tes_out;
in vec3 DOF_View_transformed;

in vec3 f_Col;
in float distFromPos;
in float dispFactor;
in float height;
in mat3 TBN_out;
in vec3 a_position_out;

// TERRAIN RELATED UNIFORMS
uniform sampler2D heightMap;
uniform sampler2D texture_background;
uniform sampler2D normalMap;

uniform float u_alpha;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform int isBarsat = 0;


uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 gEyeWorldPos;
uniform vec3 u_ViewPosition;
uniform vec3 fogColor;
uniform vec3 seed;
uniform vec2 offset;
uniform int octaves;
uniform bool drawFog;
uniform float gDispFactor;
uniform float freq;
uniform bool normals;
uniform float u_grassCoverage;
uniform float waterHeight;

uniform sampler2D sand, grass1, grass, rock, snow, rockNormal;


uniform vec3 rockColor;

float Noise(int ind, int x, int y) {

const int primes[30] = int[30](995615039, 600173719, 701464987,
					831731269, 162318869, 136250887, 
					174329291, 946737083, 245679977,
				 362489573, 795918041, 350777237, 
				 457025711, 880830799, 909678923, 
				 787070341, 177340217, 593320781,	
				 405493717, 291031019, 391950901, 
				 458904767, 676625681, 424452397, 
				 531736441, 939683957, 810651871,
				997169939, 842027887, 423882827);

	int n = x + y * 57;
	n = (n << 13) ^ n;
	int a = primes[ind*3], b = primes[ind*3 + 1], c = primes[ind*3 + 2];
	//int a = 787070341, b = 177340217, c = 593320781;
	int t = (n * (n * n * a + b) + c) & 0x7fffffff;
	return float(1.0 - t/1073741824.0);
}

float Random2D(in vec2 st)
{
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233) + seed.xy)) * 43758.5453123);
}



float InterpolatedNoise(vec2 xy) {
	float x = xy.x, y = xy.y;
	int integer_X = int(floor(x));
	float fractional_X = fract(x);
	int integer_Y = int(floor(y));
	float fractional_Y = fract(y);
	vec2 randomInput = vec2(integer_X, integer_Y);
	float a = Random2D(randomInput);
	float b = Random2D(randomInput + vec2(1.0, 0.0));
	float c = Random2D(randomInput + vec2(0.0, 1.0));
	float d = Random2D(randomInput + vec2(1.0, 1.0));

	vec2 w = vec2(fractional_X, fractional_Y);
	w = w*w*w*(10.0 + w*(-15.0 + 6.0*w));

	float k0 = a, 
	k1 = b - a, 
	k2 = c - a, 
	k3 = d - c - b + a;

	return k0 + k1*w.x + k2*w.y + k3*w.x*w.y;

}

float perlin(float x, float y, int oct){
	
	vec2 distances = vec2(500.0, 2000.0);
	int distanceFactor = int(clamp( (distances.y - distFromPos)*3.0 / (distances.y - distances.x), 0.0, 3.0));
	distanceFactor = 3 - distanceFactor;

    int numOctaves = oct;
	float persistence = 0.5;
	float total = 0,
		frequency = 0.05*freq,
		amplitude = 1.0;
	for (int i = 0; i < numOctaves; ++i) {
		frequency *= 2;
		amplitude *= persistence;
		
		total += InterpolatedNoise(vec2(x,y)*frequency) * amplitude;
	}
	return total;
}


float Interpolate(float a, float b, float x) {  // cosine interpolation
	float ft = x * 3.1415927f;
	float f = (1 - cos(ft)) * 0.5;
	return  a * (1 - f) + b * f;
}

vec4 getTexture(inout vec3 normal, const mat3 TBN){
	float trans = 20.;

	vec4 sand_t = vec4(244, 231, 127, 255)/255;//texture(sand, a_texcoord_tes_out*5.0);
	vec4 rock_t = vec4(rockColor,1.0);//texture(rock, a_texcoord_tes_out*15.0);
	vec4 grass_t = vec4(92, 196, 66, 255)/255;//texture(grass, a_texcoord_tes_out*5.0);



	sand_t = texture(sand, a_texcoord_tes_out*10.0);
	sand_t.rg *= 1.3;
	rock_t = texture(rock, a_texcoord_tes_out*vec2(1.0, 1.256).yx);
	rock_t.rgb *= vec3(2.5, 2.0, 2.0);
	grass_t = texture(grass, a_texcoord_tes_out*12.0);//*vec4(0.0, 1.5, 0.0, 1.0);
	vec4 grass_t1 = texture(grass1, a_texcoord_tes_out*12.0);//*
	float perlinBlendingCoeff = clamp(perlin(FragPos_tes_out.x, FragPos_tes_out.z, 2)*2.0 - 0.2, 0.0, 1.0);
	grass_t = mix(grass_t*1.3, grass_t1*0.75, perlinBlendingCoeff);
	grass_t.rgb *= 0.5;
	//rock_t = mix(rock_t*0.7, rock_t*0.9, perlinBlendingCoeff);
	//grass_t = vec4(grassBlendingCoeff);

	float grassCoverage = u_grassCoverage;//pow(u_grassCoverage, 0.33);

	/*
	float transMultiplier = 4.0;
	float snowHeight = gDispFactor*gDispFactor*gDispFactor*0.3 +  1800.0  - perlinBlendingCoeff*600.0*3.;
	if( FragPos_tes_out.y > snowHeight - trans*transMultiplier && FragPos_tes_out.y < snowHeight + trans*transMultiplier ){
		float gradient = clamp((FragPos_tes_out.y - (snowHeight - trans*transMultiplier))/(2.0*trans*transMultiplier), 0.0, 1.0);
		grass_t.rgb = mix(grass_t.rgb, texture(snow, a_texcoord_tes_out*5.0).rgb*1.35, gradient);
		grassCoverage = mix(grassCoverage, grassCoverage - 0.12, gradient);
	}else if(FragPos_tes_out.y > snowHeight + trans*transMultiplier){
		grass_t.rgb =texture(snow, a_texcoord_tes_out*5.0).rgb*1.35;
		grassCoverage = grassCoverage - 0.12;
	}
	*/
	vec4 heightColor;
	float cosV = abs(dot(normal, vec3(0.0, 1.0, 0.0)));
	float tenPercentGrass = grassCoverage - grassCoverage*0.1;
	float blendingCoeff = pow((cosV - tenPercentGrass) / (grassCoverage * 0.1), 1.0);

	if(height <= waterHeight + trans){
		heightColor = sand_t;
    }else if(height <= waterHeight + 2*trans){
		heightColor = mix(sand_t, grass_t, pow( (height - waterHeight - trans) / trans, 1.0));
    }else if(cosV > grassCoverage){
		heightColor = grass_t;
		mix(normal, vec3(0.0, 1.0, 0.0), 0.25);
    }else if(cosV > tenPercentGrass){
		heightColor = mix(rock_t , grass_t , blendingCoeff);
		normal = mix(TBN*(texture(rockNormal, a_texcoord_tes_out*vec2(2.0, 2.5).yx).rgb*2.0 - 1.0), normal, blendingCoeff);
    }else{
		heightColor = rock_t;
		normal = TBN*(texture(rockNormal, a_texcoord_tes_out*vec2(2.0, 2.5).yx).rgb*2.0 - 1.0);
		
	}

	return heightColor;
}

out vec4 FragColor;

vec2 ParallaxMapping(vec2 texCo, vec3 viewDr)
{
float height = texture(heightMap, texCo).r;
return(texCo - viewDr.xy * (height * 0.2));
}

uniform float fogFalloff;

const float c = 18.;
const float b = 3.e-6;

float applyFog( in vec3  rgb,      // original color of the pixel
               in float dist, // camera to point distance
               in vec3  cameraPos,   // camera position
               in vec3  rayDir )  // camera to point vector
{
    float fogAmount = c * exp(-cameraPos.y*fogFalloff) * (1.0-exp( -dist*rayDir.y*fogFalloff ))/rayDir.y;
    vec3  fogColor  = vec3(0.8706, 0.8706, 0.8706);
    return clamp(fogAmount,0.0,1.0);//mix( rgb, fogColor, fogAmount );
    //return fogAmount;//mix( rgb, fogColor, fogAmount );
}


void main(void)
{
    
    // Light Related   
    vec3 viewDir = normalize(TangentViewPos_tes_out - TangentFragPos_tes_out);
    vec2 texCoords = a_texcoord_tes_out;
	//texCoords = ParallaxMapping(texCoords, viewDir);


    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    // Calculate heightColor
    vec4 heightColor = getTexture(normal, TBN_out);

    // diffuse color
    vec3 color;
    if(isBarsat == 1)
    {
    color = texture(texture_background, texCoords).rgb * vec3(0.7 , 0.7, 0.7) ;
    color = mix(color ,  vec3(0.69411, 0.4980,0.364) , 0.2); 
    color = color *  vec3(0.6 , 0.6, 0.6); 
    }
    else
    {
        color = texture(texture_background, texCoords).rgb;
    }

    // ambient
    // vec3 ambient = 0.1 * color ;
    vec3 ambient  = 0.1 * vec3(heightColor) ;
    // diffuse
    vec3 lightDir = normalize(TangentLightPos_tes_out - TangentFragPos_tes_out);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * vec3(heightColor);
    // specular
    //vec3 viewDir = normalize(TangentViewPos_tes_out - TangentFragPos_tes_out);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;

    FragColor = heightColor;
    //FragColor = mix(heightColor ,  vec4(ambient + diffuse + specular, 1.0), 0.5 );
    //vec4 lightcolor = vec4(ambient + diffuse + specular, 1.0);
    
    //float fogFactor = applyFog(vec3(0.0), distance(gEyeWorldPos, a_position_out), gEyeWorldPos, normalize(a_position_out - gEyeWorldPos));
    //FragColor = mix(lightcolor, vec4(mix(fogColor*1.1,fogColor*0.85,clamp(a_position_out.y/(1500.*16.)*gDispFactor,0.0,1.0)), 1.0f), fogFactor);
    //FragColor = mix( heightColor, vec4(fogColor, 1.0), fogFactor );

    
    //  FragColor = vec4(color, DOF_View_transformed.z) ;
}
