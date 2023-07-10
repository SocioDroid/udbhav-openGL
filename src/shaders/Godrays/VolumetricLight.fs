#version 460 core

in vec2 texcoord_out;
// in bool isLeft;

uniform float exposure;
uniform float decay;
uniform float density;
uniform float weight;
uniform float u_illuminationDecay;
uniform vec2 lightPositionOnScreen[2];

uniform sampler2D UserMapSampler;
uniform sampler2D ColorMapSampler;

const int NUM_SAMPLES = 500 ;

out vec4 FragColor;

void main()
{	
	float decay_s=0.97815;
	float exposure_s=0.2;
	float density_s=1.5926;
	float weight_s=0.58767;

	vec4 color;
	vec4 realColor = texture2D(UserMapSampler, texcoord_out);
	color = texture2D(ColorMapSampler, texcoord_out)*0.4;
	for (int clpos=0; clpos < 2; clpos++)
	{
		// if (texcoord_out.x < (512.0/1024.0) && clpos == 1)
		// {
		// 	// if (texcoord_out.y > (512.0/1024.0))
		// 		continue;
		// 	// clpos++;
		// }
		// if (texcoord_out.x > (512.0/1024.0) && clpos == 0)
		// {
		// 	// if (texcoord_out.y > (512.0/1024.0))
		// 		continue;
		// 	// clpos++;
		// }

		vec2 textCoo = texcoord_out;
		vec2 deltaTextCoord = vec2( textCoo - lightPositionOnScreen[clpos] );
		deltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * density_s;
		float illuminationDecay = u_illuminationDecay;

		// vec4 sampleColor = texture2D(texture, texcoord_out );

		// color = texture2D(ColorMapSampler, texcoord_out)*0.4;

		for(int i=0; i < NUM_SAMPLES ; i++)
		{
				textCoo -= deltaTextCoord;
				vec4 sampleColor = texture2D(ColorMapSampler, textCoo )*0.4;
				sampleColor *= illuminationDecay * weight_s;
				color += sampleColor;
				illuminationDecay *= decay_s;
		}

	}

	FragColor = ((vec4((vec3(color.r, color.g, color.b) * exposure_s), 1.0)) + (realColor * (1.1)));
	// FragColor = vec4(isLeft, isLeft, isLeft, 1.0);
	
	// int Samples = 500;
    // float Intensity = 0.2, Decay = 0.97875;
 	// float exposure_s=0.2;

	// vec2 TexCoord = texcoord_out;
	// vec3 Color = texture2D(ColorMapSampler, TexCoord).rgb;
	// vec4 realColor = texture2D(UserMapSampler, TexCoord);

	// for(int i = 0 ; i < 2 ; i++)
	// {
	// 	// vec2 deltaTextCoord = vec2( texcoord_out - lightPositionOnScreen[i] );
	// 	// vec2 Direction = vec2( 1.419, 0.2) - TexCoord;

	// 	vec2 Direction = vec2( lightPositionOnScreen[i]) - TexCoord;

	// 	Direction /= Samples;
		
	// 	for(int Sample = 0; Sample < Samples; Sample++)
	// 	{
	// 		TexCoord += Direction;
	// 		// TexCoord -= deltaTextCoord;
	// 		Color += texture2D(ColorMapSampler, TexCoord).rgb * Intensity;
	// 		Intensity *= decay;
	// 		// Intensity *= Decay;
	// 	}	
	// }

	// FragColor = ((vec4(vec3(Color.r, Color.g, Color.b) * exposure, 1.0)) + (realColor * (1.1)));
}
