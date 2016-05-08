#version 100
precision highp float;
varying vec2 TexCoord;
// Texture samplers
uniform sampler2D Texture_diffuse_1;
uniform sampler2D Texture_specular_1;

void main()
{
	// Linearly interpolate between both textures (second texture is only slightly combined)
	gl_FragColor = vec4(texture2D(Texture_diffuse_1, TexCoord));
	//gl_FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
}