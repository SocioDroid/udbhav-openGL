#version 460 core
in vec2 texCoord;
in vec3 WorldPos;
in float distFromPos;
in float height;

uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_gEyeWorldPos;
uniform vec3 u_ViewPosition;
uniform vec3 u_fogColor;
uniform vec3 u_seed;
uniform int u_octaves;
uniform bool u_drawFog;
uniform float u_gDispFactor;
uniform float u_freq;
uniform bool u_normals;
uniform float u_grassCoverage;
uniform float u_waterHeight;
uniform float u_fogFalloff;
uniform float u_power;

uniform sampler2D sand_green, grass1_green, grass_green, rock_green, sand_dark, grass1_dark, grass_dark, rock_dark, rockNormal, snow_dark;

uniform float u_transitionFactor;

out vec4 FragColor;

float Random2D(in vec2 st) {
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233) + u_seed.xy)) * 43758.5453123);
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
	w = w * w * w * (10.0 + w * (-15.0 + 6.0 * w));

	float k0 = a, k1 = b - a, k2 = c - a, k3 = d - c - b + a;

	return k0 + k1 * w.x + k2 * w.y + k3 * w.x * w.y;

}

float perlin(float x, float y) {

	const mat2 m = mat2(0.8, -0.6, 0.6, 0.8);
	vec2 st = vec2(x, y);
	float persistence = 0.5;
	float total = 0.0, frequency = 0.005 * u_freq, amplitude = u_gDispFactor;
	for(int i = 0; i < u_octaves; ++i) {
		frequency *= 2.0;
		amplitude *= persistence;

		vec2 v = frequency * m * st;

		total += InterpolatedNoise(v) * amplitude;
	}
	return pow(total, u_power);
}

vec3 computeNormals(vec3 WorldPos, out mat3 TBN) {
	float st = 1.0;
	float dhdu = (perlin((WorldPos.x + st), WorldPos.z) - perlin((WorldPos.x - st), WorldPos.z)) / (2.0 * st);
	float dhdv = (perlin(WorldPos.x, (WorldPos.z + st)) - perlin(WorldPos.x, (WorldPos.z - st))) / (2.0 * st);

	vec3 X = vec3(1.0, dhdu, 1.0);
	vec3 Z = vec3(0.0, dhdv, 1.0);

	vec3 n = normalize(cross(Z, X));
	TBN = mat3(normalize(X), normalize(Z), n);

	//vec3 norm = mix(n, Normal, 0.5); 
	//norm = normalize(norm);
	return n;
}

vec3 ambient() {
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * u_LightColor;
	return ambient;
}

vec3 diffuse(vec3 normal) {
	vec3 lightDir = normalize(u_LightPosition - WorldPos);
	float diffuseFactor = max(0.0, dot(lightDir, normal));
	const float diffuseConst = 0.75;
	vec3 diffuse = diffuseFactor * u_LightColor * diffuseConst;
	return diffuse;
}

vec3 specular(vec3 normal) {
	vec3 lightDir = normalize(u_LightPosition - WorldPos);
	float specularFactor = 0.01;
	vec3 viewDir = normalize(u_ViewPosition - WorldPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = spec * u_LightColor * specularFactor;
	return specular;
}

float perlin(float x, float y, int oct) {

	vec2 distances = vec2(500.0, 2000.0);
	int distanceFactor = int(clamp((distances.y - distFromPos) * 3.0 / (distances.y - distances.x), 0.0, 3.0));
	distanceFactor = 3 - distanceFactor;

	int numOctaves = oct;
	float persistence = 0.5;
	float total = 0, frequency = 0.05 * u_freq, amplitude = 1.0;
	for(int i = 0; i < numOctaves; ++i) {
		frequency *= 2;
		amplitude *= persistence;

		total += InterpolatedNoise(vec2(x, y) * frequency) * amplitude;
	}
	return total;
}

vec4 getTexture(inout vec3 normal, const mat3 TBN) {
	float trans = 20.;

	vec4 sand_t = texture(sand_green, texCoord * 10.0);
	sand_t.rg *= 1.3;
	vec4 rock_t = texture(rock_green, texCoord * vec2(1.0, 1.256).yx);
	rock_t.rgb *= vec3(2.5, 2.0, 2.0);
	vec4 grass_t = texture(grass_green, texCoord * 12.0);
	vec4 grass_t1 = texture(grass1_green, texCoord * 12.0);//*
	float perlinBlendingCoeff = clamp(perlin(WorldPos.x, WorldPos.z, 2) * 2.0 - 0.2, 0.0, 1.0);
	grass_t = mix(grass_t * 1.3, grass_t1 * 0.75, perlinBlendingCoeff);
	grass_t.rgb *= 0.5;

	float grassCoverage = u_grassCoverage;

	vec4 heightColor;
	float cosV = abs(dot(normal, vec3(0.0, 1.0, 0.0)));
	float tenPercentGrass = grassCoverage - grassCoverage * 0.1;
	float blendingCoeff = pow((cosV - tenPercentGrass) / (grassCoverage * 0.1), 1.0);

	if(height <= u_waterHeight + trans) {
		heightColor = sand_t;
	} else if(height <= u_waterHeight + 2 * trans) {
		heightColor = mix(sand_t, grass_t, pow((height - u_waterHeight - trans) / trans, 1.0));
	} else if(cosV > grassCoverage) {
		heightColor = grass_t;
		mix(normal, vec3(0.0, 1.0, 0.0), 0.25);
	} else if(cosV > tenPercentGrass) {
		heightColor = mix(rock_t, grass_t, blendingCoeff);
		normal = mix(TBN * (texture(rockNormal, texCoord * vec2(2.0, 2.5).yx).rgb * 2.0 - 1.0), normal, blendingCoeff);
	} else {
		heightColor = rock_t;
		normal = TBN * (texture(rockNormal, texCoord * vec2(2.0, 2.5).yx).rgb * 2.0 - 1.0);
	}
	return heightColor;
}

vec4 getDarkTexture(inout vec3 normal, const mat3 TBN) {
	float trans = 20.;

	vec4 sand_t = texture(sand_dark, texCoord * 10.0);
	vec4 rock_t = texture(rock_dark, texCoord * vec2(1.0, 1.256).yx);
	vec4 grass_t = texture(grass_dark, texCoord * 12.0);
	vec4 grass_t1 = texture(grass1_dark, texCoord * 12.0);
	float perlinBlendingCoeff = clamp(perlin(WorldPos.x, WorldPos.z, 2) * 2.0 - 0.2, 0.0, 1.0);
	grass_t = mix(grass_t * 1.3, grass_t1 * 0.75, perlinBlendingCoeff);
	grass_t.rgb *= 0.5;

	float grassCoverage = u_grassCoverage;

	float transMultiplier = 4.0;
	float snowHeight = u_gDispFactor * u_gDispFactor * u_gDispFactor * 0.3 + 1800.0 - perlinBlendingCoeff * 600.0 * 3.;
	if(WorldPos.y > snowHeight - trans * transMultiplier && WorldPos.y < snowHeight + trans * transMultiplier) {
		float gradient = clamp((WorldPos.y - (snowHeight - trans * transMultiplier)) / (2.0 * trans * transMultiplier), 0.0, 1.0);
		grass_t.rgb = mix(grass_t.rgb, texture(snow_dark, texCoord * 5.0).rgb * 1.35, gradient);
		grassCoverage = mix(grassCoverage, grassCoverage - 0.12, gradient);
	} else if(WorldPos.y > snowHeight + trans * transMultiplier) {
		grass_t.rgb = texture(snow_dark, texCoord * 5.0).rgb * 1.35;
		grassCoverage = grassCoverage - 0.12;
	}

	vec4 heightColor;
	float cosV = abs(dot(normal, vec3(0.0, 1.0, 0.0)));
	float tenPercentGrass = grassCoverage - grassCoverage * 0.1;
	float blendingCoeff = pow((cosV - tenPercentGrass) / (grassCoverage * 0.1), 1.0);

	if(height <= u_waterHeight + trans) {
		heightColor = sand_t;
	} else if(height <= u_waterHeight + 2 * trans) {
		heightColor = mix(sand_t, grass_t, pow((height - u_waterHeight - trans) / trans, 1.0));
	} else if(cosV > grassCoverage) {
		heightColor = grass_t;
		mix(normal, vec3(0.0, 1.0, 0.0), 0.25);
	} else if(cosV > tenPercentGrass) {
		heightColor = mix(rock_t, grass_t, blendingCoeff);
		normal = mix(TBN * (texture(rockNormal, texCoord * vec2(2.0, 2.5).yx).rgb * 2.0 - 1.0), normal, blendingCoeff);
	} else {
		heightColor = rock_t;
		normal = TBN * (texture(rockNormal, texCoord * vec2(2.0, 2.5).yx).rgb * 2.0 - 1.0);

	}

	return heightColor;
}

float applyFog(
	in float dist, // camera to point distance
	in vec3 cameraPos,   // camera position
	in vec3 rayDir
)  // camera to point vector
{
	const float c = 18.;
	float fogAmount = c * exp(-cameraPos.y * u_fogFalloff) * (1.0 - exp(-dist * rayDir.y * u_fogFalloff)) / rayDir.y;
	return clamp(fogAmount, 0.0, 1.0);
}

void main() {
	// calculate fog color 
	bool normals_fog = true;
	float fogFactor = applyFog(distance(u_gEyeWorldPos, WorldPos), u_gEyeWorldPos, normalize(WorldPos - u_gEyeWorldPos));
	float eps = 0.1;

	vec3 n;
	mat3 TBN;
	if(u_normals && normals_fog) {
		n = computeNormals(WorldPos, TBN);
		n = normalize(n);
	} else {
		n = vec3(0, 1, 0);
	}

	vec4 heightColor = mix(getDarkTexture(n, TBN), getTexture(n, TBN), u_transitionFactor);

	vec3 ambient = ambient();
	vec3 diffuse = diffuse(n);
	vec3 specular = specular(n);

	// putting all together
	vec4 color = heightColor * vec4((ambient + specular * 0 + diffuse) * vec3(1.0), 1.0);
	if(u_drawFog) {
		FragColor = mix(color, vec4(mix(u_fogColor * 1.1, u_fogColor * 0.85, clamp(WorldPos.y / (1500. * 16.) * u_gDispFactor, 0.0, 1.0)), 1.0), fogFactor);
		FragColor.a = WorldPos.y / u_waterHeight;
	} else {
		FragColor = color;
		FragColor.a = WorldPos.y / u_waterHeight;
	}

	//FragColor.rgb = n*0.5 + 0.5;
}
