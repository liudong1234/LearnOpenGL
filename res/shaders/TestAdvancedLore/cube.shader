#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 aTexCoord;

layout(std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform mat4 model;

//out vec2 TexCoord;
out VS_OUT
{
	vec2 TexCoord;
}vs_out;

void main()
{
	vs_out.TexCoord = aTexCoord;
	gl_Position = projection * view * model * vec4(position, 1.0f);
	gl_PointSize = gl_Position.z;
}



#shader fragment
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