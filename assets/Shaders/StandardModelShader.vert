#version 100
attribute vec3 position;
attribute vec3 normal;
attribute vec2 texture;

varying vec2 TexCoord;

uniform mat4 local;
uniform mat4 model_view;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model_view * vec4(position, max(normal.x,1.0f));
    TexCoord = texture;
}