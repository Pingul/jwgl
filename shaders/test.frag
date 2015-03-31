#version 150

in vec3 normal;
out vec4 outColor;

void main(void)
{
	const float Ia = 0.2;
	const float Kd = 0.9;
	vec3 ambientLight = Kd*Ia*vec3(1.0, 1.0, 1.0);

	const vec3 light = vec3(0.58, 0.58, 0.58);
	const float Is = 0.7;
	vec3 diffuseLight = Kd*Is*max(0, dot(normal, normalize(light)))*vec3(1.0, 1.0, 1.0);

	outColor = vec4(ambientLight + diffuseLight, 1.0);
}
