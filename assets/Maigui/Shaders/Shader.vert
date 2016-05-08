#version 100
attribute vec3 position;
attribute vec2 texCoord;
attribute vec2 texCoord2;

varying vec2 TexCoord;
varying vec2 TexCoord2;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	TexCoord = texCoord;
	TexCoord2 = texCoord2;
};