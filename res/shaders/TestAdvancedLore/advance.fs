#version 330 core

in VS_OUT
{
	vec2 TexCoord;
}fs_in;

out vec4 FragColor;


uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
	if (gl_FrontFacing)
		FragColor = texture(tex, fs_in.TexCoord);
	else
		FragColor = texture(tex2, fs_in.TexCoord);
		//FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	//gl_FragDepth = gl_FragCoord.z + 0.1;
}