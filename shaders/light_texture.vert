#version 150

uniform mat4 projectionMatrix;
uniform mat4 MTW;
uniform mat4 WTV;

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TextureCoordinates;

out vec3 surface;
out vec3 normal;
out vec2 textureCoordinates;

void main(void)
{
	normal = mat3(WTV*MTW)*in_Normal;
	surface = vec3(WTV*MTW*vec4(in_Position, 1.0));
	textureCoordinates = in_TextureCoordinates;
	gl_Position = projectionMatrix*WTV*MTW*vec4(in_Position, 1.0);
}

