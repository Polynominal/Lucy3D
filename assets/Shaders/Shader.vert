#version 100
attribute vec3 position;
attribute vec2 texCoord2;

varying vec2 TexCoord;

uniform mat4 transformation;

uniform mat4 local;
uniform mat4 model_view;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model_view  * vec4(position, 1.0f);
    TexCoord = texCoord2;
}