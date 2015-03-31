#version 150

uniform mat4 projectionMatrix;
uniform mat4 MTW;
uniform mat4 WTV;

in vec3 in_Position;
in vec3 in_Normal;

out vec3 normal;

void main(void)
{
	// vec3 rotatedNormals = normalize(mat3(rotationMatrix)*in_Normal);
	normal = in_Normal;
	gl_Position = projectionMatrix*WTV*MTW*vec4(in_Position, 1.0);
}
