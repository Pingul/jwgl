#version 150

const int LIGHT_SOURCES = 2;

uniform vec3 lightSourceDirectionPositions[LIGHT_SOURCES];
uniform vec3 lightSourceColor[LIGHT_SOURCES];
uniform float specularExponent[LIGHT_SOURCES];
uniform bool isDirectional[LIGHT_SOURCES];

uniform mat4 WTV;

in vec3 normal;
in vec3 surface;

out vec4 outColor;

void main(void)
{
	vec3 normal_n = normalize(normal);

	const float Ka = 0.2f;
	vec3 ambientLight = Ka*vec3(1.0f, 1.0f, 1.0f);

	vec3 diffuseLight = vec3(0.0f, 0.0f, 0.0f);
	vec3 specularLight = vec3(0.0f, 0.0f, 0.0f);

	const float Kd = 0.4f;
	const float Ks = 1.0f;

	for (int i = 0; i < LIGHT_SOURCES; i++)
	{	
		if (isDirectional[i])
		{
			vec3 rotatedLightSource = mat3(WTV)*lightSourceDirectionPositions[i];

			float cosineIncomingAngle = dot(normal_n, rotatedLightSource);
			diffuseLight += Kd*max(0, cosineIncomingAngle)*lightSourceColor[i];

			vec3 reflectedDirection = normalize(reflect(-rotatedLightSource, normal_n));
			vec3 viewDirection = -normalize(surface);
			specularLight += Ks*pow(max(0, dot(reflectedDirection, viewDirection)), specularExponent[i])*lightSourceColor[i];
		}
		else
		{
			vec3 rotatedLightSource = vec3(WTV*vec4(lightSourceDirectionPositions[i], 1.0));
			vec3 lightDirection = normalize(surface - rotatedLightSource);

			float cosineIncomingAngle = dot(normal_n, -lightDirection);
			diffuseLight += Kd*max(0, cosineIncomingAngle)*lightSourceColor[i];

			vec3 reflectedDirection = normalize(reflect(lightDirection, normal_n));
			vec3 viewDirection = -normalize(surface);
			specularLight += Ks*pow(max(0, dot(reflectedDirection, viewDirection)), specularExponent[i])*lightSourceColor[i];			
		}
	}
	outColor = vec4(ambientLight + diffuseLight + specularLight, 1.0f);
}
