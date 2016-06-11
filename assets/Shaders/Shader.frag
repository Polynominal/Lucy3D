#version 100
precision highp float;
varying vec2 TexCoord;
// Texture samplers
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	// Linearly interpolate between both textures (second texture is only slightly combined)
	gl_FragColor = mix(texture2D(ourTexture1, TexCoord), texture2D(ourTexture2, TexCoord), 0.2);
	if (TexCoord.x < 0.1f || TexCoord.x > 0.9f || 
	    TexCoord.y < 0.1f || TexCoord.y > 0.9f)
	{gl_FragColor = vec4(1.0f,0.0f,0.0f,0.0f);}
	else{gl_FragColor = vec4(0.0f,0.0f,0.0f,0.0f);};
	//gl_FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
}