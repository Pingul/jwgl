#version 150

const int LIGHT_SOURCES = 2;

uniform vec3 lightSourceDirectionOrPosition[LIGHT_SOURCES];
uniform vec3 lightSourceColor[LIGHT_SOURCES];
uniform float specularExponent[LIGHT_SOURCES];
uniform bool isDirectional[LIGHT_SOURCES];

uniform mat4 WTV;

uniform sampler2D TEXTURE_UNIT_0;

in vec2 textureCoordinates;
in vec3 normal;
in vec3 surface;

out vec4 outColor;

void main(void)
{
	vec3 normal_n = normalize(normal);

	const float Ka = 0.0f;
	vec4 textureColor = Ka*texture(TEXTURE_UNIT_0, textureCoordinates);

	vec3 specularLight = vec3(0.0f, 0.0f, 0.0f);
	vec3 diffuseLight = vec3(0.0f, 0.0f, 0.0f);

	const float Kd = 0.4f;
	const float Ks = 1.0f;

	for (int i = 0; i < LIGHT_SOURCES; i++)
	{	
		vec3 rotatedLightSource;
		if (isDirectional[i])
			rotatedLightSource = mat3(WTV)*lightSourceDirectionOrPosition[i];
		else
			rotatedLightSource = vec3(WTV*vec4(lightSourceDirectionOrPosition[i], 1.0));

		vec3 light = surface - rotatedLightSource;
		vec3 lightDirection = normalize(light);

		float cosineIncomingAngle = dot(normal_n, -lightDirection);
		diffuseLight += Kd/*/pow(length(light), 0.25)*/*max(0, cosineIncomingAngle)*lightSourceColor[i];

		vec3 reflectedDirection = normalize(reflect(lightDirection, normal_n));
		vec3 viewDirection = -normalize(surface);
		specularLight += Ks/*/pow(length(light), 0.25)*/*pow(max(0, dot(reflectedDirection, viewDirection)), specularExponent[i])*lightSourceColor[i];			
	}

	outColor = textureColor + vec4(specularLight + diffuseLight, 1.0f);
	// outColor = vec4(isDirectional[0]);
}
